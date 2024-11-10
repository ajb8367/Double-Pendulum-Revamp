#ifndef PENDULUM_OBJ_H
#define PENDULUM_OBJ_H

#include <random>

// Namespace containing the Double Pendulum objects
namespace DoublePendulum {

	//
	class PendulumObject {
	private:
		// Fields

		double time;
		double gravity;
		double massrat;
		double aArray[18];
		double ang1, ang2;
		double vel1, vel2;
		double acc1, acc2;
		double len1, len2;



	public:
		// ACCESSORS

		double getTime();
		double getLengthOne();
		double getLengthTwo();
		double getAngleOne();
		double getAngleTwo();
		double getVelocityOne();
		double getVelocityTwo();
		double getAccelOne();
		double getAccelTwo();


		// MUTATORS
		/* setPosition - sets the position of one of the pendulum bobs
		* [int] selector - selects the bob, 1 being the inner, 2 being the outer, other values give an error
		* [int] x - the x position of the bob relative to the center of the screen
		* [int] y - the y position of the bob relative to the center of the screen
		*/
		void setPosition(int selector, int x, int y);

		/* step - steps the double pendulum simulation
		* [double] interval - the step interval, assumed to be positive
		* returns [double] - the incremented time, for display or differential calculations
		*/
		double step(double interval);

		/* setToRest - resets the system to rest, but preserves the angles and lengths.
		*/
		void setToRest();

		/* randReset - Resets the entire system with a random radius as reference
		* [int] rad - A reference radius for the random generation
		*/
		void randReset(int rad);


		// PUBLIC INSTANCE METHODS

		// PUBLIC STATIC METHODS

		/* angleModTwoPi - takes in an angle argument and takes mod 2pi
		* [double] angle - the angle to rollover
		* returns [double] - the angle rolled over to be within (-pi, pi]
		*/
		static double angleModTwoPi(double angle);

	};

}


#endif