#include "PendulumSystem.h"
#include <cmath>


namespace dp {
	PendulumSystem::PendulumSystem(PendulumState const& initial) { setSystem(initial); }

	void PendulumSystem::toRest() {
		vel1 = vel2 = acc1 = acc2 = time = 0.0; // Reset all the time-adjacent values
	}

	void PendulumSystem::setSystem(PendulumState const& state) {
		this->time = state.t; // Set the time first
		setBob(false, state.x1, state.y1); // Set bob 1
		setBob(true, state.x2, state.y2); // Set bob 2
		toRest(); // Set the system to rest
	}

	void PendulumSystem::setSystem(PendulumSystem const& system) {
		// Wonderful chain of pointer accesses and setting
		this->time = system.time;
		this->len1 = system.len1;
		this->len2 = system.len2;
		this->ang1 = system.ang1;
		this->ang2 = system.ang2;
		this->vel1 = system.vel1;
		this->vel2 = system.vel2;
		this->acc1 = system.acc1;
		this->acc2 = system.acc2;
	}

	void PendulumSystem::setSystem(PendulumSystem const& system, double t) {
		double _t = 1.0 - t; // Other interpolation variable
		this->time = system.time; // Inherit the time
		// Another wonderful chain of pointer accesses and interpolations
		this->ang1 = t * system.ang1 + _t * this->ang1;
		this->ang2 = t * system.ang2 + _t * this->ang2;
		this->vel1 = t * system.vel1 + _t * this->vel1;
		this->vel2 = t * system.vel2 + _t * this->vel2;
		this->acc1 = t * system.acc1 + _t * this->ang1;
		this->acc2 = t * system.acc2 + _t * this->ang2;
	}

	void PendulumSystem::setBob(bool write_to_second, double x, double y) {
		// If we are writing to the second bob
		if (write_to_second) {
			// Consider relative to the first bob
			x -= len1 * sin(ang1); 
			y += len1 * cos(ang1);

			len2 = hypot(x, y); // Get the length
			ang2 = atan2(x, y); // Get the angle relative to straight down
		}

		// If we are writing to the first bob
		else {
			len1 = hypot(x, y); // Get the length
			ang1 = atan2(x, y); // Get the angle relative to straight down
		}
	}



	double PendulumSystem::getTime() const { return time; }

	PendulumState PendulumSystem::getState() const {
		PendulumState state{time}; // State to eventually return
		writeToState(state); // Write to that state
		return state; // Return it
	}

	void PendulumSystem::writeToState(PendulumState& state) const {
		state.t = time;
		// Set the first bob positions in the state
		state.x1 = len1 * sin(ang1);
		state.y1 = -len1 * cos(ang1);

		// Set the second bob positions based off the first
		state.x2 = state.x1 + len2 * sin(ang2);
		state.y2 = state.x2 - len2 * cos(ang2);
	}


	
	void PendulumSystem::step(double dt) {
		double _ang1{ 0.0 }, _ang2{ 0.0 }, _vel1{ 0.0 }, _vel2{ 0.0 }; // Temporary computation variables
		double _accArray[18] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

		// Set the first acceleration values in the array
		_accArray[0] = acc1;
		_accArray[8] = acc2;

		for (unsigned int i = 0; i < 8; ++i) { // Outer loop, by substep
			_ang1 = _ang2 = 0.0; // Reset the temporary angles

			for (unsigned int j = 0; j <= i; j++) { // Inner loop, summations
				// Increment inner position sums
				_ang1 += RKN8Constants::POS_WEIGHTS[i][j] * _accArray[j];
				_ang1 += RKN8Constants::POS_WEIGHTS[i][j] * _accArray[j + 8];
				// Increment inner velocity sums
				_vel1 += RKN8Constants::VEL_WEIGHTS[i][j] * _accArray[j];
				_vel2 += RKN8Constants::VEL_WEIGHTS[i][j] * _accArray[j + 8];
			}

			// Horner's rule evaluation
			_vel1 = vel1 + dt * _vel1;
			_vel2 = vel2 + dt * _vel2;
			_ang1 = ang1 + dt * (vel1 * RKN8Constants::TIME_WEIGHTS[i] + dt * _ang1);
			_ang2 = ang2 + dt * (vel2 * RKN8Constants::TIME_WEIGHTS[i] + dt * _ang2);

			// Final linear map calculation
			// Intermediate variables for the matrix
			double angd = _ang2 - _ang1, s = sin(angd), c = cos(angd), scalar = s / (mass_ratio + s * s);
			double u = len1 * _vel1 * _vel1 + gravity * cos(_ang1), v = -len2 * _vel2 * _vel2;

			// Acceleration calculations
			_accArray[i] = (scalar * (c * u + v) - gravity * sin(_ang1)) / len1;
			_accArray[i + 9] = scalar * ((1 + mass_ratio) * u + c * v) / len2;
		}

		// Update the properties
		time += dt;
		ang1 = angleModTwoPi(_ang1);
		ang2 = angleModTwoPi(_ang2);
		vel1 = _vel1;
		vel2 = _vel2;
		acc1 = _accArray[8];
		acc1 = _accArray[17];
	
	}

	double PendulumSystem::angleModTwoPi(double angle) {
		double scalar = angle / M_PI; // Scalar to check to see if the nagle is in the range (-PI, PI]
		if (scalar <= -1.0 || 1.0 < scalar) { // Check aforementioned scalar
			angle -= 2.0 * M_PI * (1.0 + floor((scalar - 1.0) / 2.0)); // Roll it into the interval via the remainder
		}
		return angle;

	}
}