#ifndef MATRIX_CONSTANTS_H
#define MATRIX_CONSTANTS_H

/*Constants for the RK8N method*/
namespace RKN8Constants {
	//For debugging, the standard step length
	static const double STD_STEP = 0.00005;

	// Cumulative time
	static constexpr double TIME_TOTAL = 4.0 / 15 + 2.0 / 5 + 3.0 / 5 + 9.0 / 10 + 3.0 / 4 + 2.0 / 7 + 1.0 + 1.0;

	// Time weights for the Runge-Kutta-Nystrom method
	static constexpr double TIME_WEIGHTS[8] = { 4.0 / 15, 2.0 / 5, 3.0 / 5, 9.0 / 10, 3.0 / 4, 2.0 / 7, 1.0, 1.0 };

	// Velocity weights for the Runge-Kutta-Nystrom method
	static constexpr double VEL_WEIGHTS[8][8] = {
		{4.0 / 15, 0, 0, 0, 0, 0, 0, 0},
		{1.0 / 10, 3.0 / 10, 0, 0, 0, 0, 0, 0},
		{3.0 / 20, 0, 9.0 / 20, 0, 0, 0, 0, 0},
		{9.0 / 40, 0, 0, 27.0 / 40, 0, 0, 0, 0},
		{11.0 / 48, 0, 0, 5.0 / 8, -5.0 / 48, 0, 0, 0},
		{27112.0 / 194481, 0, 0, 56450.0 / 64827, 80000.0 / 194481, -24544.0 / 21609, 0, 0},
		{-26033.0 / 41796, 0, 0, -236575.0 / 38313, -14500.0 / 10449, 275936.0 / 45279, 228095.0 / 73788, 0},
		{7.0 / 81, 0, 0, 0, -250.0 / 3483, 160.0 / 351, 2401.0 / 5590, 1.0 / 10}
	};

	// Position weights for the Runge-Kutta-Nystrom method
	static constexpr double POS_WEIGHTS[8][8] = {
		{8.0 / 225, 0, 0, 0, 0, 0, 0, 0},
		{1.0 / 25, 1.0 / 25, 0, 0, 0, 0, 0, 0},
		{9.0 / 160, 81.0 / 800, 9.0 / 400, 0, 0, 0, 0, 0},
		{81.0 / 640, 0, 729.0 / 3200, 81.0 / 1600, 0, 0, 0, 0},
		{11283.0 / 88064, 0, 3159.0 / 88064, 7275.0 / 44032, -33.0 / 688, 0, 0, 0},
		{6250.0 / 194481, 0, 0, 0, -3400.0 / 194481, 1696.0 / 64827, 0, 0},
		{-6706.0 / 45279, 0, 0, 0, 1047925.0 / 1946997, -147544.0 / 196209,1615873.0 / 1874886, 0},
		{31.0 / 360,0,0,0,0, 64.0 / 585, 2401.0 / 7800, -1.0 / 300}
	};

}


#endif