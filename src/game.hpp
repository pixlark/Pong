#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

enum GameState {

	PLAYING,
	PAUSED,
	GAME_OVER

};

enum BallState {

	REGULAR,
	SLOWDOWN

};

struct Paddle {

	sf::RectangleShape * paddle;
	sf::Vector2f size;
	float speed = 1000;
	float offset = 50;

};

struct Ball {

	sf::RectangleShape * ball;
	float speed;
	float next_speed;
	float base_speed = 800;
	sf::Vector2f direction;
	sf::Vector2f size;
	BallState ball_state;

};

struct SlowdownBall {

	sf::CircleShape * shape;
	bool enabled;
	float cooldown;

};

extern GameState game_state;

void Initialize();
void Update(float delta_time, sf::RenderWindow * window);
void Draw(sf::RenderWindow * window);

#endif
