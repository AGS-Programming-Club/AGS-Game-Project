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

#include "config.h"

#include "ProtectableAllocator.h"

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#elif HAVE_WINDOWS_H
#include <Windows.h>
#endif

#include <Box2D/Common/b2Settings.h>

ProtectableAllocator::ProtectableAllocator(size_t size) : size(size), used(0) {
    void* mem;
#if HAVE_SYS_MMAN_H
    mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#elif HAVE_WINDOWS_H
    mem = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
#else
    mem = NULL
#endif
    this->mem = (uint8_t*) mem;
}

ProtectableAllocator::~ProtectableAllocator() {
#if HAVE_SYS_MMAN_H
    munmap(mem, size);
#elif HAVE_WINDOWS_H
    VirtualFree(mem, 0, MEM_RELEASE);
#endif
}

void* ProtectableAllocator::Allocate(size_t length, size_t align) {
    b2Assert(mem != NULL);
    b2Assert((align & (align - 1)) == 0); // Assert is power of 2
    if (used & (align - 1) != 0) used = used & ~(align - 1) + 1; // Align address
    void* ptr = mem + used;
    used += length;
    b2Assert(used <= size);
    return ptr;
}

void ProtectableAllocator::Free(void* ptr) {
    b2Assert(mem != NULL);
    // Free is not supported, so we just ignore it.
    B2_NOT_USED(ptr);
}

void ProtectableAllocator::Protect(AccessLevel access) {
    b2Assert(mem != NULL);
#if HAVE_SYS_MMAN_H
    int prot;
    switch (access) {
    case AccessLevel::NOACCESS: prot = PROT_NONE; break;
    case AccessLevel::READONLY: prot = PROT_READ; break;
    case AccessLevel::READWRITE: prot = PROT_READ | PROT_WRITE; break;
    }
    int ret = mprotect(mem, size, prot);
    b2Assert(ret == 0);
#elif HAVE_WINDOWS_H
    int protect;
    switch (access) {
    case AccessLevel::NOACCESS: protect = PAGE_NOACCESS; break;
    case AccessLevel::READONLY: protect = PAGE_READONLY; break;
    case AccessLevel::READWRITE: protect = PAGE_READWRITE; break;
    }
    int ret = VirtualProtect(mem, size, protect, NULL);
    b2Assert(ret != 0);
#endif
}

static void* b2PageAlloc(int32 size, void* self) {
    return ((ProtectableAllocator*) self)->Allocate(size);
}

static void b2PageFree(void* ptr, void* self) {
    ((ProtectableAllocator*) self)->Free(ptr);
}

void ProtectableAllocator::SetAs_b2Alloc() {
    b2SetAllocFreeCallbacks(b2PageAlloc, b2PageFree, this, true);
}
