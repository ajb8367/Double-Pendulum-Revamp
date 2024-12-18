#define _USE_MATH_DEFINES // So I can use the math constants
#ifndef PEND_SYS_GUARD
#define PEND_SYS_GUARD

#include "PendulumState.h"
#include "RKN8Matrix.h"

namespace dp {
	// Class that actually simulates the system using a Runge-Kutta-Nystrom method
	class PendulumSystem {
	public:
		/* Constructor off of the provided initial pendulum state
		* \param initial - The initial PendulumState
		*/
		PendulumSystem(PendulumState const& initial);
		


		// Set the system to rest, setting the derivatives and time to 0;
		void toRest();

		/* Set the values of the system off of a state, then it is set to rest
		* \param state - The state that the system is to be set to
		*/
		void setSystem(PendulumState const& state);


		/* Set the values of the system off of a system, then it is set to rest
		* \param system - The system that the system is to be set to
		*/
		void setSystem(PendulumSystem const& system);

		/* Interpolate the system towards the provided system
		* \param system - The system that the system is to be set to
		*/
		void setSystem(PendulumSystem const& system, double t);


		/* Get the time of the system
		* \return The accumulated time of the system
		*/ 
		double getTime() const;

		/* Get the state of the system
		* \return The state of the system, consisting of the time and the Cartesian bob positions 
		*/
		PendulumState getState() const;
		

		/* Write the current system to a state object
		* \param state - A reference to the state to be overwritten
		*/
		void writeToState(PendulumState& state) const;

		/* Increment the system by a given small time interval
		* \param dt - The amount of time to increment the system by
		*/
		void step(double dt); 
		
		/* Take a floating-point angle and take it mod 2PI to normalize it to the interval (-PI, PI]
		* \param angle - the angle to apply the modulus to
		*/
		static double angleModTwoPi(double angle);


	private:

		double time; // Time of the system

		double mass_ratio = 2.0; // Mass ratio of the second bob to the first bob
		double gravity = 27807.879; // Gravity of the system in pixels/second^2

		double len1; // Length to the first bob
		double ang1; // Angle of the first bob to the vertical of the center
		double vel1; // Angular velocity of the first bob to the vertical of the center
		double acc1; // Angular acceleration of the first bob to the vertical of the center


		double len2; // Length of the second bob to the first bob
		double ang2; // Angle of the second bob to the vertical of the first bob 
		double vel2; // Angular velocity of the second bob to the vertical of the first bob
		double acc2; // Angular acceleration of the second bob to the vertical of the first bob


		/* Set the Cartesian coordinates of one of the bobs in the system 
		* \param write_to_second - Boolean indicating true if setting the second bob, false if setting the first
		* \param x - The new x coordinate
		* \param y - The new y coordinate
		*/
		void setBob(bool write_to_second, double x, double y);

	};



}

#endif