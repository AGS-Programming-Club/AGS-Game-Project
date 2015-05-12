#ifndef TIMER
#define TIMER

namespace timing {
	class Ticker {
	private:
		int tickNo;
		long tickDeltas [20];

		long prevousTick;

	public:
		void tick();
		int getFrames();
		int lastDelta();
		double averageDelta();
		double TPS();

		Ticker() {
			tickNo = 0;

			for(int i = 0; i < 20; i++)
				tickDeltas[i] = 0;

			prevousTick = 0;
		}

	};

	Ticker& getFrameClock();
}

#endif
