#include <iostream>
#include <sstream>
#include <SFML\Graphics.hpp>
#include "PendulumState.h"
#include "PendulumGraphics.h"

const unsigned int WINDOW_START_WIDTH = 800;
const unsigned int WINDOW_START_HEIGHT = 600;





int main() {
	


	// Creating a window and a view
	sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT), "Double Pendulum"); // Creating a new window


	// Creating a starting origin and system with random values
	sf::Vector2f start_center{ static_cast<float>(WINDOW_START_WIDTH) / 2.f, static_cast<float>(WINDOW_START_HEIGHT) / 2.f };
	dp::PendulumState start_state{ 0.0, 300.0,0.0,300.0,250.0 };


	// The pain of loading in a damn font
	sf::Font consolas;
	if (!consolas.loadFromFile("consola.ttf")) { abort(); }

	// Creating a graphics object
	dp::PendulumGraphics pendulum{ start_center, &start_state , consolas};


	// Mouse drag tomfoolery
	bool mouse_held = false; // Is the mouse held?
	int selected = -1; 

	// Reset key check
	bool reset_held = false; // is the reset key held



	while (window.isOpen()) { // Loop while the window is open

		// Event catching loop
		sf::Event event;
		while (window.pollEvent(event)) { // Read all the pooled events
			if (event.type == sf::Event::Closed) { window.close(); } // Closed event
			if (event.type == sf::Event::Resized) { // Resized event
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				pendulum.setCenter(sf::Vector2f{ event.size.width / 2.f, event.size.height / 2.f }); // Recenter the pendulum
			}


			// Mouse detection tomfoolery
			if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left) && !mouse_held) { // If left click and it isn't being currently held
				std::cout << "CLICK!!!!" << std::endl;
				mouse_held = true; // It is currently being held
				sf::Vector2f mouse_pos{ sf::Mouse::getPosition(window) }; // Get the mouse position relative to the top left of the window
				selected = pendulum.inCircle(mouse_pos); // See what pendulum it is in
			}

			if ((event.type == sf::Event::MouseButtonReleased) && (event.mouseButton.button == sf::Mouse::Left) && mouse_held) { mouse_held = false; selected = -1; }  // If left click RELEASE

			if (!mouse_held && (event.type == sf::Event::KeyPressed) && (event.key.scancode == sf::Keyboard::Scancode::R) && !reset_held) { // If the reset is not held
				std::cout << "RESET!!!!" << std::endl;
				reset_held = true; // Reset is being held
				double height_increment = static_cast<double>(window.getSize().y) / 6.f; 
				dp::PendulumState reset_state(0.0, 0.0, -height_increment, 0.0, -2.0 * height_increment);
				pendulum.setSystem(&reset_state);

			}
			if (reset_held && (event.type == sf::Event::KeyReleased) && (event.key.scancode == sf::Keyboard::Scancode::R)) { reset_held = false; } // Release the reset
		}

		if (mouse_held) {
			sf::Vector2f mouse_pos{ sf::Mouse::getPosition(window) }; // Get the mouse position relative to the top left of the window
			if (selected == 1) { pendulum.setBob(false, mouse_pos); } // Set the first bob to the cursor position
			else if (selected == 2) { pendulum.setBob(true, mouse_pos); } // Set the second bob to the cursor position
		}


		// Drawing part
		window.clear(sf::Color::White); // Clear the screen to white
		window.draw(pendulum); // Draw the pendulum object
		window.display(); // Display the screen
	
	}

	// Exit the program
	std::cout << "Window closed!" << std::endl;
	return 0;
}