#include "PendulumGraphics.h"
#include <sstream>


namespace dp {
	PendulumGraphics::PendulumGraphics(sf::Vector2f const& origin, PendulumState* initial, sf::Font& text_font) : time_text{"",text_font}, center_circle { BUTTON_RADIUS }, bob1_circle{BOB_ONE_RADIUS}, bob2_circle{BOB_TWO_RADIUS} {
		// Setting up the time text
		time_text.setFillColor(sf::Color::Black);
		time_text.setCharacterSize(20);
		time_text.setPosition(0.f, 0.f);


		// Shifting the origins of the circles to their actual centers
		center_circle.setOrigin(BUTTON_RADIUS, BUTTON_RADIUS);
		center_circle.setFillColor(sf::Color::Black);
		bob1_circle.setOrigin(BOB_ONE_RADIUS, BOB_ONE_RADIUS);
		bob1_circle.setFillColor(sf::Color::Blue);
		bob2_circle.setOrigin(BOB_TWO_RADIUS, BOB_TWO_RADIUS);
		bob2_circle.setFillColor(sf::Color::Red);
		
		// Calling the other methods to fully set up the object
		system_center = origin;
		setSystem(initial);
	}

	int PendulumGraphics::inCircle(sf::Vector2f const& point) const {
		//Check center
		sf::Vector2f vector(point - system_center);
		if (vector.x * vector.x + vector.y * vector.y <= BUTTON_RADIUS * BUTTON_RADIUS) {
			return 0;
		}

		//Bob 1 check
		vector = point - bob1_position;
		if (vector.x * vector.x + vector.y * vector.y <= BOB_ONE_RADIUS * BOB_ONE_RADIUS) {
			return 1;
		}

		//Bob 2 check
		vector = point - bob2_position;
		if (vector.x * vector.x + vector.y * vector.y <= BOB_TWO_RADIUS * BOB_TWO_RADIUS) {
			return 2;
		}

		return -1;
	}

	void PendulumGraphics::setSystem(PendulumState* state) {
		// Convert the values from the state into vectors
		time = state->t;
		bob1_position.x = static_cast<float>(state->x1);
		bob1_position.y = -static_cast<float>(state->y1); // Swapped due to top-left corner convention
		bob2_position.x = static_cast<float>(state->x2);
		bob2_position.y = -static_cast<float>(state->y2);

		// Affine shift to the center
		bob1_position += system_center;
		bob2_position += system_center;

		updateObjects(); // Update the drawings
	}

	void PendulumGraphics::setCenter(sf::Vector2f const& origin) {
		// Make the bobs relative to the top left corner of the screen
		bob1_position -= system_center;
		bob2_position -= system_center;

		//Recenter the bobs
		system_center = origin;
		bob1_position += origin;
		bob2_position += origin;

		updateObjects(); // Updating the objects
	}

	void PendulumGraphics::setBob(bool moving_second, sf::Vector2f const& new_pos) {
		if (moving_second) { bob2_position = mapInBounds(new_pos); } // If relocating the second bob
		else { bob1_position = mapInBounds(new_pos); } // Otherwise relocating the first
		updateObjects(); // Update the objects
	}

	void PendulumGraphics::setCenterColor(sf::Color const& color) { center_circle.setFillColor(color);}

	void PendulumGraphics::writeToState(dp::PendulumState* state) {
		// Set the time
		state->t = time;

		// Some relative vectors
		sf::Vector2f relative1 = bob1_position - system_center;
		sf::Vector2f relative2 = bob2_position - system_center;

		// Set the values
		state->x1 = relative1.x;
		state->y1 = -relative1.y;
		state->x2 = relative2.x;
		state->y2 = -relative2.y;

	}


	sf::Vector2f PendulumGraphics::mapInBounds(sf::Vector2f const& vector) {
		sf::Vector2f duplicate = vector;
		if (duplicate.x < 0.f) { duplicate.x = 0.f; } // Left cap
		else {
			float width = system_center.x * 2.f;
			if (duplicate.x > width) { duplicate.x = width; } // Right cap
		}
		if (duplicate.y < 0.f) { duplicate.y = 0.f;  } // Top cap
		else {
			float height = system_center.y * 2.f;
			if (duplicate.y > height) { duplicate.y = height; } // Bottom cap
		}

		return duplicate;
	}



	void PendulumGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const {

		// Drawing the objects
		target.draw(bob_line, 3, sf::LineStrip);
		target.draw(center_circle);
		target.draw(bob1_circle);
		target.draw(bob2_circle);
		target.draw(time_text);

		//std::cout << time << std::endl;
	}


	void PendulumGraphics::updateObjects() {
		// Set the circle centers, since they're tossed in as copies
		center_circle.setPosition(system_center);
		bob1_circle.setPosition(bob1_position);
		bob2_circle.setPosition(bob2_position);


		// Update the verticies
		bob_line[0] = sf::Vertex(system_center, sf::Color::Black);
		bob_line[1] = sf::Vertex(bob1_position, sf::Color::Black);
		bob_line[2] = sf::Vertex(bob2_position, sf::Color::Black);

		// Change the time text
		std::ostringstream oss;
		oss << "Time: " << time << "s" << std::endl;
		time_text.setString(oss.str());

	}

}