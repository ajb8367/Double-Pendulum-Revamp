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

		int getx1();
		int gety1();
		int getx2();
		int gety2();

	};
}
#endif