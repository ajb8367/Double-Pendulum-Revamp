#include <iostream>
#include <sstream>
#include <SFML\Graphics.hpp>
#include "PendulumState.h"
#include "PendulumGraphics.h"

const unsigned int WINDOW_START_WIDTH = 800;
const unsigned int WINDOW_START_HEIGHT = 600;



int main() {
	

	// The pain of loading in a damn font
	sf::Font consolas;
	if (!consolas.loadFromFile("consola.ttf")) { abort(); }

	// Creating a window and a view
	sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT), "Double Pendulum"); // Creating a new window


	// Creating a starting origin and system with random values
	sf::Vector2f start_center{ static_cast<float>(WINDOW_START_WIDTH) / 2.f, static_cast<float>(WINDOW_START_HEIGHT) / 2.f };
	dp::PendulumState start_state{ 0.0, 300.0,0.0,300.0,250.0 };

	// Creating a graphics object
	dp::PendulumGraphics pendulum{ start_center, &start_state };


	// Debugging text
	sf::Text text;
	text.setString("Hello world!");
	text.setFont(consolas);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);

	while (window.isOpen()) { // Loop while the window is open

		// Event catching loop
		sf::Event event; 
		while (window.pollEvent(event)) { // Read all the pooled events
			if (event.type == sf::Event::Closed) { window.close(); } // Closed event
			if (event.type == sf::Event::Resized) { // Resized event
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
				pendulum.setCenter(sf::Vector2f{ event.size.width / 2.f, event.size.height / 2.f }); // Recenter the pendulum
			} 
		}

		// Cursor position debug
		sf::Vector2f mouse_pos{ sf::Mouse::getPosition(window)};
		int selection = pendulum.inCircle(mouse_pos);

		std::ostringstream oss;
		oss << "( " << mouse_pos.x << ", " << mouse_pos.y << ")";
		text.setString(oss.str());



		// Debug switch for color
		switch (selection) {
			case 0:
				text.setFillColor(sf::Color::Green);
				break;
			case 1:
				text.setFillColor(sf::Color::Blue);
				break;
			case 2:
				text.setFillColor(sf::Color::Red);
				break;
			default:
				text.setFillColor(sf::Color::Black);
		}

		// Drawing part
		window.clear(sf::Color::White); // Clear the screen to white
		window.draw(text);
		window.draw(pendulum); // Draw the pendulum object
		window.display(); // Display the screen
	
	}

	// Exit the program
	std::cout << "Window closed!" << std::endl;
	return 0;
}