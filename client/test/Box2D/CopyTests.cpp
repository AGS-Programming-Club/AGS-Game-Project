/*
 *  Copyright (c) 2015, Andrew Chen <llkiwi2006@gmail.com>
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the copyright holder nor the names of its
 *  contributors may be used to endorse or promote products derived from this
 *  software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "gtest/gtest.h"

#include "Box2D/Box2D.h"

#include "ProtectableAllocator.hpp"

/**
 * Test the b2World copy function (implemented through a copy constructor)
 *
 * Test outline:<br>
 * - Check parameters are the same on both worlds after copying.
 * - Check doing stuff the new world does not access the old world.
 * - Check simulating the old and new world yield the same results.
 * - Check making a new copy of the world on each step of the simulation works.
 */
class CopyTests : public ::testing::Test {
    protected:
    virtual void SetUp();
    virtual void TearDown();

    static constexpr float32 TIME_STEP = 1.0f / 60.0f;
    static constexpr int32 ITERATIONS = 240;
    static constexpr int32 VELOCITY_ITERATIONS = 8;
    static constexpr int32 POSITION_ITERATIONS = 3;
    static constexpr int32 PARTICLE_ITERATIONS = 3;

    static constexpr int ALLOC_SIZE = 1024 * 1024;

    ProtectableAllocator* old_allocator;
    ProtectableAllocator* new_allocator;

    b2World *old_world;
    b2Body *old_body;

    b2World *new_world;
    b2Body *new_body;
};

void CopyTests::SetUp() {
    old_allocator = new ProtectableAllocator(ALLOC_SIZE);
    old_allocator->SetAs_b2Alloc();
    old_world = (b2World*) old_allocator->Allocate(sizeof(b2World));

    b2Vec2 gravity(0.0f, -10.0f);
    new (old_world) b2World(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = old_world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(1.23456789f, 4.56789f);
    bodyDef.angularVelocity = 0.987654321f;
    old_body = old_world->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.11111f, 2.22222f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.7f;
    old_body->CreateFixture(&fixtureDef);

    old_allocator->Protect(AccessLevel::READONLY);


    new_allocator = new ProtectableAllocator(ALLOC_SIZE);
    new_allocator->SetAs_b2Alloc();
    new_world = (b2World*) new_allocator->Allocate(sizeof(b2World));

    new (new_world) b2World(old_world);

    new_body = new_world->GetBodyList();
}

void CopyTests::TearDown() {
    new_allocator->SetAs_b2Alloc();
    new_allocator->Protect(AccessLevel::READWRITE);
    new_world->~b2World();
    delete new_allocator;

    old_allocator->SetAs_b2Alloc();
    old_allocator->Protect(AccessLevel::READWRITE);
    old_world->~b2World();
    delete old_allocator;

    b2SetAllocFreeCallbacks(NULL, NULL, NULL);
}

TEST_F(CopyTests, InitialParametersTest) {
    ASSERT_NE(new_world, old_world);
    ASSERT_NE(new_body, old_body);

    ASSERT_EQ(old_body->GetType(), b2_dynamicBody);
    ASSERT_EQ(new_body->GetType(), b2_dynamicBody);

    b2Vec2 oldPosition = old_body->GetPosition();
    float32 oldAngle = old_body->GetAngle();

    old_allocator->Protect(AccessLevel::NOACCESS);

    EXPECT_EQ(new_body->GetPosition(), oldPosition);
    EXPECT_EQ(new_body->GetAngle(), oldAngle);

    old_allocator->Protect(AccessLevel::READONLY);
}

TEST_F(CopyTests, SimulationTest) {
    old_allocator->Protect(AccessLevel::READWRITE);
    new_allocator->Protect(AccessLevel::NOACCESS);

    b2Vec2 oldPosition = old_body->GetPosition();
    float32 oldAngle = old_body->GetAngle();

    for (int32 i = 0; i < ITERATIONS; i++) {
        old_world->Step(TIME_STEP,
                VELOCITY_ITERATIONS, POSITION_ITERATIONS, PARTICLE_ITERATIONS);
    }

    old_allocator->Protect(AccessLevel::NOACCESS);
    new_allocator->Protect(AccessLevel::READWRITE);

    EXPECT_EQ(new_body->GetPosition(), oldPosition);
    EXPECT_EQ(new_body->GetAngle(), oldAngle);

    for (int32 i = 0; i < ITERATIONS; i++) {
        new_allocator->Protect(AccessLevel::READONLY);
        ProtectableAllocator* step_allocator = new ProtectableAllocator(ALLOC_SIZE);
        step_allocator->SetAs_b2Alloc();
        b2World* step_world = (b2World*) step_allocator->Allocate(sizeof(b2World));
        new (step_world) b2World(new_world);
        new_allocator->Protect(AccessLevel::NOACCESS);

        step_world->Step(TIME_STEP,
                VELOCITY_ITERATIONS, POSITION_ITERATIONS, PARTICLE_ITERATIONS);

        new_allocator->SetAs_b2Alloc();
        new_allocator->Protect(AccessLevel::READWRITE);
        new_world->~b2World();
        delete new_allocator;

        step_allocator->SetAs_b2Alloc();
        new_allocator = step_allocator;
        new_world = step_world;
        new_body = step_world->GetBodyList();
    }

    old_allocator->Protect(AccessLevel::READONLY);

    EXPECT_EQ(new_body->GetPosition(), old_body->GetPosition());
    EXPECT_EQ(new_body->GetAngle(), old_body->GetAngle());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
