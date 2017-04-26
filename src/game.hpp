#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

enum GameState {

	PLAYING,
	PAUSED

};

extern GameState game_state;

void Initialize();
void Update(float delta_time);
void Draw(sf::RenderWindow * window);

#endif
