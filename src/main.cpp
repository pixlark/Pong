#include <time.h>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Game", sf::Style::Close, settings);
	window.setMouseCursorVisible(false);
	window.setMouseCursorGrabbed(true);

	Initialize();

	sf::Clock delta_time_clock;
	bool running = true;
	while (running) {
	
		float delta_time = delta_time_clock.getElapsedTime().asSeconds();
		delta_time_clock.restart();

		sf::Event window_event;
		while (window.pollEvent(window_event)) {
		
			switch (window_event.type) {
			
				case sf::Event::Closed: {
				
					running = false;

				} break;

				case sf::Event::MouseButtonPressed: {
				
					if (window_event.mouseButton.button == sf::Mouse::Left) {
					
						input_bits |= ILEFTCLICK;

					}

				} break;

				case sf::Event::MouseButtonReleased: {

					if (window_event.mouseButton.button == sf::Mouse::Left) {

						input_bits &= ~ILEFTCLICK;

					}

				} break;

				case sf::Event::KeyPressed: {
				
					if (window_event.key.code == sf::Keyboard::Escape) {
						running = false;
					}

					if (window_event.key.code == sf::Keyboard::W || window_event.key.code == sf::Keyboard::Up) {
						input_bits |= IFORWARDS;
					}

					if (window_event.key.code == sf::Keyboard::A || window_event.key.code == sf::Keyboard::Left) {
						input_bits |= ILEFT;
					}

					if (window_event.key.code == sf::Keyboard::S || window_event.key.code == sf::Keyboard::Down) {
						input_bits |= IBACKWARDS;
					}

					if (window_event.key.code == sf::Keyboard::D || window_event.key.code == sf::Keyboard::Right) {
						input_bits |= IRIGHT;
					}

					if (window_event.key.code == sf::Keyboard::Return) {
						input_bits |= IRETURN;
					}

				} break;

				case sf::Event::KeyReleased: {

					if (window_event.key.code == sf::Keyboard::Escape) {
						running = false;
					}

					if (window_event.key.code == sf::Keyboard::W || window_event.key.code == sf::Keyboard::Up) {
						input_bits &= ~IFORWARDS;
					}

					if (window_event.key.code == sf::Keyboard::A || window_event.key.code == sf::Keyboard::Left) {
						input_bits &= ~ILEFT;
					}

					if (window_event.key.code == sf::Keyboard::S || window_event.key.code == sf::Keyboard::Down) {
						input_bits &= ~IBACKWARDS;
					}

					if (window_event.key.code == sf::Keyboard::D || window_event.key.code == sf::Keyboard::Right) {
						input_bits &= ~IRIGHT;
					}

					if (window_event.key.code == sf::Keyboard::Return) {
						input_bits &= ~IRETURN;
					}

				} break;

			}

		}

		Update(delta_time, &window);

		window.clear();
		Draw(&window);
		window.display();

	}
	
}
