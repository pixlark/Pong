#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

struct Paddle {

	sf::RectangleShape * paddle;
	sf::Vector2f size;
	float speed = 1000;
	float offset = 50;

};

struct Ball {

	sf::RectangleShape * ball;
	float speed = 700;
	float next_speed;
	sf::Vector2f direction;
	sf::Vector2f size;

};

enum GameState {

	PLAYING,
	PAUSED,
	GAME_OVER

};

extern GameState game_state;

void Initialize();
void Update(float delta_time, sf::RenderWindow * window);
void Draw(sf::RenderWindow * window);

#endif
