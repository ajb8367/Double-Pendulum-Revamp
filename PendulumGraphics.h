#ifndef PEND_GRAPH_GUARD
#define PEND_GRAPH_GUARD

#include <SFML/Graphics.hpp>
#include "PendulumState.h"

namespace dp {

	const float BUTTON_RADIUS = 15.f; // Constant for the center button radius
	const float BOB_ONE_RADIUS = 25.f; // Constant for the first bob radius
	const float BOB_TWO_RADIUS = 20.f; // Constant for the second bob radius





	// Class for drawing a pendulum, extends off of SFML's drawable object class
	class PendulumGraphics : public sf::Drawable {
	public: 

		/* Standard object constructor taking in a center and a pointer to the state to start from
		* \param origin - The initial center of the system
		* \param initial_pntr - The pointer to the state to write from
		*/
		PendulumGraphics(sf::Vector2f const& origin, PendulumState* initial_pntr);

		/* Returns what circle (if any) a point is in
		* \param point - The point to check to see what circle it lies in
		* \return An integer where -1 if not in any, 0 if in the button, 1 if in bob 1, and 2 if in bob 2
		*/
		int inCircle(sf::Vector2f const& point) const;

		/* Sets the system from a given pendulum state
		* \param state_pntr - The pointer to the state to write from
		*/
		void setSystem(PendulumState* state_ptr); 

		/* Sets the center of the system and adjusts other values
		* \param origin - The new origin of the system
		*/
		void setCenter(sf::Vector2f const& origin);

		/* Sets the color of the center button
		* \param color - The color to set to
		*/
		void setCenterColor(sf::Color const& color);

	private:
		double time = 0.0;

		sf::Vector2f system_center; // Origin
		sf::Vector2f bob1_position; // Position of the first bob
		sf::Vector2f bob2_position; // Position of the second bob

		sf::Vertex bob_line[3]; //Lines to act as the 'strings'

		sf::CircleShape center_circle; // Center button circle
		sf::CircleShape bob1_circle; // Bob 1's circle
		sf::CircleShape bob2_circle; // Bob 2's circle



		// Virtual drawing function to draw the pendulum to the screen
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		
		// Private method to update the drawn objects from the vectors
		void updateObjects();

	};
}



#endif
