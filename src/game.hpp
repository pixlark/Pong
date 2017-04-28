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
	float powerup_cooldown;

};

struct SlowdownBall {

	sf::CircleShape * shape;

	bool enabled; // Able to be picked up
	float power_cooldown; // How long the powerup lasts
	float max_lifetime; // How long it stays in the arena
	float max_spawn_cooldown; // How long until it respawns
	float speed_effect; // The effect it has on ball speed

	float spawn_cooldown; // How much longer until it can spawn again
	float lifetime; // How much longer until it disappears from the arena

};

extern GameState game_state;

void Initialize();
void Update(float delta_time, sf::RenderWindow * window);
void Draw(sf::RenderWindow * window);

#endif
