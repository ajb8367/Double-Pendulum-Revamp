#ifndef PENDULUM_DATA_H
#define PENDULUM_DATA_H


namespace DoublePendulum {
	class PendulumData {
	private:
		// FIELDS

		double time;
		int x1;
		int y1;
		int x2;
		int y2;

	public:
		// ACCESSORS 

		double getTime();
		int getX1();
		int getY1();
		int getX2();
		int getY2();

		// MUTATORS

		/* overwriteData - Update/overwrite the pendulum data with time and cartesian values
		* [double] time - the current time of the system
		* [int] x1 - Cartesian x value of the center of bob 1
		* [int] y1 - Cartesian y value of the center of bob 1
		* [int] x2 - Cartesian x value of the center of bob 2
		* [int] y2 - Cartesian y value of the center of bob 2
		*/
		void overwriteData(double time, int x1, int y1, int x2, int y2);
		

		// INSTANCE METHODS
		
		/* inBob - Checks to see if the given cartesian coordinates lie within the two bobs provided the ghost datum
		* [int] argx - The abscissa of the point
		* [int] argy - The ordinate of the point
		* returns [int] - 0 if it doesn't lie in a point, 1 if in bob 1, 2 if in bob 2
		*/
		int inBob(int argx, int argy);
	};
}
#endif