
#ifndef PEND_OBJ_GUARD
#define PEND_OBJ_GUARD


#include <random>
#include "RKN8Matrix.h"
#include "PendulumPositionStruct.h"

// Namespace containing the Double Pendulum objects
namespace dp {
	const double MASS_RATIO = 2.0;
	const double GRAVITY = 9.81;

	// The class for the actual pendulum object itself
	class PendulumObject {
	private:
		// Fields

		double time = 0.00;
		double gravity = 98100;
		double massrat = 1;
		double ang1, ang2;
		double vel1, vel2;
		double acc1, acc2;
		double len1, len2;



	public:
		// CONSTRUCTORS


		// ACCESSORS

		double getTime() const;
		PendulumPositionStruct getPositions() const;


		// MUTATORS

		void setPosition(int selector, int x, int y);
		void setToRest();

		void randReset(int rad);


		// PUBLIC INSTANCE METHODS

		double step(double interval);

		// PUBLIC STATIC METHODS


		static double angleModTwoPi(double angle);

	};

}


#endif