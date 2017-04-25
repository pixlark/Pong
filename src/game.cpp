#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "constants.hpp"

sf::Texture my_texture;
sf::Sprite my_sprite;

void Initialize() {

	my_texture.loadFromFile("../../resources/grunt.png");
	my_sprite.setTexture(my_texture);

	my_sprite.setPosition(constants::SCREEN_WIDTH / 2, constants::SCREEN_HEIGHT / 2);
	sf::FloatRect ms_bounds = my_sprite.getGlobalBounds();
	my_sprite.setOrigin(ms_bounds.width / 2, ms_bounds.height / 2);

}

void Update(float delta_time) {

	my_sprite.setRotation(my_sprite.getRotation() + 10 * delta_time);

}

void Draw(sf::RenderWindow * window) {

	window->draw(my_sprite);

}
