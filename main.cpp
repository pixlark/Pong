#include <SFML/Graphics.hpp>

int main() {

	sf::RenderWindow window(sf::VideoMode(1366, 768), "Title", sf::Style::Fullscreen);
	window.setMouseCursorVisible(false);

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

				case sf::Event::Closed: {

					window.close();
					
				} break;

				case sf::Event::KeyPressed: {

					if (event.key.code == sf::Keyboard::Escape) {

						window.close();
						
					}
					
				} break;
				
			}
			
		}

		window.clear(sf::Color::White);
		window.display();
		
	}
	
}
