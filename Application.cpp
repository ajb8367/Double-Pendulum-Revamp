#include <iostream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <SFML\Graphics.hpp>
#include "PendulumState.h"
#include "PendulumGraphics.h"
#include "PendulumSystem.h"

// Starting parameters
const unsigned int WINDOW_START_WIDTH = 800;
const unsigned int WINDOW_START_HEIGHT = 600;


// Atomic boolean flags
std::atomic<bool> running{ true }; // Is it running?
std::atomic <bool> paused{ false }; // Is the system paused?
std::atomic<bool> calc_waiting{ false }; // Has the calculation thread started waiting?

// Pause Mutex System
std::mutex pause_sync; // Mutex to sync the calculation and main graphics threads
std::condition_variable pause_cv; // Condition variable to pause the calculation thread

// Double-Buffer pointer array
dp::PendulumState* buffer[2];





// Constants describing the max framerate, the standard steps and the time intervals
const int MAX_FRAME_RATE = 144;
const std::chrono::nanoseconds STANDARD_STEP{ 1000L };
const std::chrono::nanoseconds FRAME_INTERVAL{ 1000000000L / MAX_FRAME_RATE };
const double STEPF = static_cast<double>(STANDARD_STEP.count()) / 1000000000.0;

// Calculation thread loop
void calc_thread_loop(dp::PendulumState initial_state) {

	// The two pendulum systems, one leading and one trailing for interpolation
	dp::PendulumSystem  past_system{ initial_state }, present_system{initial_state};

	std::chrono::steady_clock::time_point past = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point present = std::chrono::steady_clock::now();

	std::chrono::nanoseconds time_diff{ 0L }, relative_diff{ 0L }; // Two time intervals for calculations


	// While running
	while (running.load()) {

		// If paused
		if (paused.load()) {
			std::unique_lock<std::mutex> lock{ pause_sync }; // Gain the lock
			calc_waiting.store(true); // Store that it is waiting
			pause_cv.wait(lock, [] {return !paused.load(); });
			calc_waiting.store(false); // Reset the flag, after notified
			past = std::chrono::steady_clock::now(); // Get a time point for reference

			// Set the systems
			past_system.setSystem(*buffer[1]);
			present_system.setSystem(*buffer[1]);

			time_diff = relative_diff = std::chrono::nanoseconds{ 0L }; // Reset the intervals
		}

		present = std::chrono::steady_clock::now(); // Get the current time point
		time_diff = std::chrono::duration_cast<std::chrono::nanoseconds>(present - past);

		if (time_diff > FRAME_INTERVAL) { time_diff = FRAME_INTERVAL; } // Bound by the minimum

		relative_diff += time_diff;
		past = std::chrono::steady_clock::now(); // Set the past time point


		// Loop until the second to last
		while (relative_diff >= 2L * STANDARD_STEP) {
			past_system.step(STEPF);
			relative_diff -= STANDARD_STEP;
		}
		present_system.setSystem(past_system);
		present_system.step(STEPF);

		present_system.setSystem(past_system, static_cast<double>(relative_diff.count()) / STANDARD_STEP.count()); // Interpolate


		present_system.writeToState(buffer[0]); // Write to the state

		// Swap the pointers
		dp::PendulumState* hold_ptr = buffer[0];
		buffer[0] = buffer[1];
		buffer[1] = hold_ptr;
	

	}

}











// Main thread function
int main() {
	// Creating a window and a view
	sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT), "Double Pendulum"); // Creating a new window


	// Creating a starting origin and system with random values
	sf::Vector2f start_center{ static_cast<float>(WINDOW_START_WIDTH) / 2.f, static_cast<float>(WINDOW_START_HEIGHT) / 2.f };
	dp::PendulumState start_state{ 0.0, 0.0,-150.0,0.0,-250.0};


	// The pain of loading in a damn font
	sf::Font consolas;
	if (!consolas.loadFromFile("consola.ttf")) { abort(); }

	// Creating a graphics object
	dp::PendulumGraphics pendulum{ start_center, &start_state , consolas};


	// Create the buffer states in the heap
	buffer[0] = new dp::PendulumState(start_state);
	buffer[1] = new dp::PendulumState(start_state);

	std::thread calc_thread{ [&start_state] {calc_thread_loop(start_state); } };


	// Mouse drag tomfoolery
	bool mouse_held = false; // Is the mouse held?
	int selected = -1; 

	// Reset key check
	bool reset_held = false; // is the reset key held

	// Unpausing flag
	bool unpausing = false; // does the calculation thread need woken up? 

	while (window.isOpen()) { // Loop while the window is open

		if (unpausing && calc_waiting.load()) { // If we have yet to awaken the other block
			{ // Synced
				std::lock_guard<std::mutex> guard{ pause_sync }; // Get possession of the mutex, synchronize the block
				paused.store(false); // Unpause
			}
			unpausing = false;
			pendulum.writeToState(buffer[1]);
			pause_cv.notify_one(); // Wake up the other thread
		}

		// Event catching loop
		sf::Event event;
		while (window.pollEvent(event)) { // Read all the pooled events
			if (event.type == sf::Event::Closed) { window.close(); running.store(false); calc_thread.join(); } // Closed event
			if (event.type == sf::Event::Resized) { // Resized event
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				pendulum.setCenter(sf::Vector2f{ event.size.width / 2.f, event.size.height / 2.f }); // Recenter the pendulum
			}


			// Mouse detection tomfoolery
			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left) && !mouse_held) { // If left click and it isn't being currently held
				mouse_held = true; // It is currently being held
				sf::Vector2f mouse_pos{ sf::Mouse::getPosition(window) }; // Get the mouse position relative to the top left of the window
				selected = pendulum.inCircle(mouse_pos); // See what pendulum it is in


				// Pausing initiation
				if (selected == 0) {
					if (!paused.load()) { // If pausing
						std::lock_guard<std::mutex> guard{ pause_sync }; // Get possession of the mutex, synchronize the block
						paused.store(true); // Pause the system
					}

					else { // if unpausing
						unpausing = true;
					}
				}
			}

			if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left) && mouse_held) { mouse_held = false; selected = -1; }  // If left click RELEASE




			// Reset checks
			if (!mouse_held && (event.type == sf::Event::KeyPressed) && (event.key.scancode == sf::Keyboard::Scancode::R) && !reset_held) { // If the reset is not held
				reset_held = true; // Reset is being held
				double height_increment = static_cast<double>(window.getSize().y) / 6.f; 
				dp::PendulumState reset_state(0.0, 0.0, -height_increment, 0.0, -2.0 * height_increment);
				pendulum.setSystem(&reset_state);

			}
			if (reset_held && (event.type == sf::Event::KeyReleased) && (event.key.scancode == sf::Keyboard::Scancode::R)) { reset_held = false; } // Release the reset
		}


		// Unpaused branch
		if (!paused.load()) 
		{
			pendulum.setSystem(buffer[1]);
		}
		else if (mouse_held) {
			sf::Vector2f mouse_pos{ sf::Mouse::getPosition(window) }; // Get the mouse position relative to the top left of the window
			if (selected == 1) { pendulum.setBob(false, mouse_pos); } // Set the first bob to the cursor position
			else if (selected == 2) { pendulum.setBob(true, mouse_pos); } // Set the second bob to the cursor position
		}


		// Drawing part
		window.clear(sf::Color::White); // Clear the screen to white
		window.draw(pendulum); // Draw the pendulum object
		window.display(); // Display the screen
	
	}

	// Delete the buffers in the heap
	delete buffer[0], buffer[1];

	// Exit the program
	std::cout << "Window closed!" << std::endl;
	return 0;
}


// https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75