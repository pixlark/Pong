#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

enum GameState {

	PLAYING,
	PAUSED

};

enum ControlType {

	KEYBOARD,
	MOUSE

};

extern GameState game_state;
extern ControlType control_type;

void Initialize();
void Update(float delta_time, sf::RenderWindow * window);
void Draw(sf::RenderWindow * window);

#endif
