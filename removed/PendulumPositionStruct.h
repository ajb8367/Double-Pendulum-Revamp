#ifndef PEND_POS_STR
#define PEND_POS_STR


namespace dp {
	// A struct allowing for the publically-accessible storage of bob positions and the current time of the system.
	struct PendulumPositionStruct {
		double time = 0.0;
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

	public:
		// Default construct for the PendulumPositionStruct struct
		PendulumPositionStruct(double time, int x1, int y1, int x2, int y2) {
			this->time = time;
			this->x1 = x1;
			this->x2 = x2;
			this->y1 = y1;
			this->y2 = y2;
		};

		// Empty constructor, initializes everything to 0.0
		PendulumPositionStruct() {}
	};
}
#endif