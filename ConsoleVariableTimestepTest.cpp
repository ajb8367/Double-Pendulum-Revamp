


#include <iostream>
#include "PendulumObject.h"
#include <chrono>

using namespace DoublePendulum;

int main()
{
    std::cout << "Bozo test one" << std::endl;

    const unsigned int CALCULATIONS_PER_SECOND = 60;
    constexpr std::chrono::duration<int, std::ratio<1000000, CALCULATIONS_PER_SECOND>> STANDARD_PERIOD{ 1 };
    unsigned int count;
    double initialStep = 0.001;

    // Initialize two time_point objects representing different points in time
    std::chrono::system_clock::time_point timeOne;
    std::chrono::system_clock::time_point timeTwo;
    std::chrono::system_clock::time_point ORIGIN = std::chrono::system_clock::now();

    // Durations that store the amount of time elapsed between calculations for the adaptive step process
    // Standard assumed calculation time, just as a good starting point via assumption.

    PendulumObject pendulum;
    pendulum.setPosition(1, 20, -20);
    pendulum.setPosition(2, 0, -40);

    timeOne = std::chrono::system_clock::now();
    pendulum.step(STANDARD_PERIOD.count() / 1000000.0);
    timeTwo = std::chrono::system_clock::now();
    auto timeDelta = std::chrono::duration_cast<std::chrono::microseconds>(timeTwo - timeOne);
    for (unsigned int i = 0; i < 300000; i++) {
        timeOne = std::chrono::system_clock::now();
        pendulum.step(timeDelta.count() / 1000000.0);
        if (i % 1000 == 0) {
            PendulumPositionStruct ioStruct = pendulum.getPositions();
            std::cout << ioStruct.time << "s - (" << ioStruct.x1 << ", " << ioStruct.y1 << ") & (" << ioStruct.x2 << ", " << ioStruct.y2 << ")" << std::endl;
        }
        timeTwo = std::chrono::system_clock::now();
        timeDelta = std::chrono::duration_cast<std::chrono::microseconds>(timeTwo - timeOne);
    }

}

/* REFERENCES
* 1) https://learn.microsoft.com/en-us/cpp/standard-library/chrono?view=msvc-170
* 2) https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75
*/
