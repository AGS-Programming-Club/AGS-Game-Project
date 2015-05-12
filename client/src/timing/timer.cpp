#include "timing/timer.hpp"
#include <GLFW/glfw3.h>

namespace timing {
	double Ticker::averageDelta() {
		double total = 0;

		if(tickNo < 20) {
			for(int i = 0; i < tickNo; i++) {
				total += tickDeltas[i];
			}

			return total / tickNo;
		} else {
			for(int i = 0; i < 20; i++) {
				total += tickDeltas[i];
			}

			return total / 20.0;
		}
	}

	void Ticker::tick() {
		long time = long(glfwGetTime() * 1000);
		tickDeltas[tickNo++ % 20] = time - prevousTick;
		prevousTick = time;
	}

	int Ticker::getFrames() {
		return tickNo;
	}

	int Ticker::lastDelta() {
		return tickDeltas[(tickNo - 1) % 20];
	}

	double Ticker::TPS() {
		return 1000.0 / averageDelta();
	}

	Ticker frameClock = Ticker{};

	Ticker& getFrameClock() {
		return frameClock;
	}
}
