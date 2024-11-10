#define _USE_MATH_DEFINES // I spent 30 minutes trying to get this macro to work but it was in the wrong spot... i hate this
#include <cmath>


#include "PendulumObject.h"


namespace DoublePendulum {

	// CONSTRUCTOR


	// ACCESSORS

	double PendulumObject::getTime() { return time; }
	double PendulumObject::getLengthOne() { return len1; }
	double PendulumObject::getLengthTwo() { return len2; }
	double PendulumObject::getAngleOne() { return ang1; }
	double PendulumObject::getAngleTwo() { return ang2; }
	double PendulumObject::getVelocityOne() { return vel1; }
	double PendulumObject::getVelocityTwo() { return vel2; }
	double PendulumObject::getAccelOne() { return acc1; }
	double PendulumObject::getAccelTwo() { return acc2; }

	// MUTATORS

	void PendulumObject::setPosition(int selector, int x, int y) {}
	void PendulumObject::setToRest() {}
	void PendulumObject::randReset(int rad) {}

	// PUBLIC INSTANCE METHODS

	double PendulumObject::step(double interval) {
		// Temporary variables for the intermediate angles and velocities
		double tang1 = 0, tvel1 = 0;
		double tang2 = 0, tvel2 = 0;


		// Set the first two acceleration values in the array.
		aArray[0] = acc1;
		aArray[9] = acc2;


		// Outer loop
		for (int i = 1; i <= 8; i++) {

			tang1 = tvel1 = tang2 = tvel2 = 0; // Reset the temporary variables


			// Inner loop for the temporary time and velocity summations
			for (int j = 0; j < i; j++) {
				tang1 += RKN8Constants::POS_WEIGHTS[i - 1][j] * aArray[j];
				tang2 += RKN8Constants::POS_WEIGHTS[i - 1][j] * aArray[j + 9];
				tvel1 += RKN8Constants::VEL_WEIGHTS[i - 1][j] * aArray[j];
				tvel2 += RKN8Constants::VEL_WEIGHTS[i - 1][j] * aArray[j + 9];
			}

			// Full evaluation of temporary position and velocity terms via Horner's Rule
			tang1 = interval * ((interval * tang1) + (vel1 * RKN8Constants::TIME_WEIGHTS[i - 1])) + ang1;
			tang2 = interval * ((interval * tang2) + (vel2 * RKN8Constants::TIME_WEIGHTS[i - 1])) + ang2;
			tvel1 = (interval * tvel1) + vel1;
			tvel2 = (interval * tvel2) + vel2;


			// Intermediate variables for the 
			double angd = tang2 - tang1, s = sin(angd), c = cos(angd), det = s / (massrat + s * s);
			double x = len1 * tvel1 * tvel1 + gravity * cos(tang1), y = len2 * tvel2 * tvel2;

			// Acceleration calculations
			aArray[i] = (det * (c * x + y) - gravity * sin(tang1)) / len1;
			aArray[i + 9] = -1 * det * ((1 + massrat) * x + c * y) / len2;
		}

		// Update the fields
		time += interval;
		ang1 = angleModTwoPi(tang1);
		vel1 = tvel1;
		acc1 = aArray[8];
		ang2 = angleModTwoPi(tang2);
		vel2 = tvel2;
		acc2 = aArray[17];

		//Return the time just in case it needs to be used.
		return time;

	}

	// PUBLIC STATIC METHODS

	double PendulumObject::angleModTwoPi(double angle) {
		double scalar = angle / (M_PI); // Scalar to check to see if angle is in (-pi, pi]
		if (scalar <= -1 || 1 < scalar) {
			angle -= 2 * M_PI * (1.0 + floor((scalar - 1) / 2)); // Finds the modulus centered about 0
		}
		return angle;
	}
}