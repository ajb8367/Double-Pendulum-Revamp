#ifndef PEND_STATE_GUARD
#define PEND_STATE_GUARD

namespace dp {

	// Struct containing the position and time data of the system at a given moment
	struct PendulumState {
		double t; // Current time of the system
		double x1; // x-position of the first bob
		double y1; // y-position of the first bob
		double x2; // x-position of the second bob
		double y2; // y-position of the second bob


		// Standard initializing constructor
		PendulumState(double at = 0, double ax1 = 0, double ay1 = 0, double ax2 = 0, double ay2 = 0) : t(at), x1(ax1), x2(ax2), y1(ay1), y2(ay2) {};
	};


}

#endif