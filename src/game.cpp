#include <sstream>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"

GameState game_state;

sf::RectangleShape paddle_left;
sf::RectangleShape paddle_right;
sf::Vector2f paddle_size(30, 200);
float paddle_speed = 750;
float paddle_offset = 50;

sf::RectangleShape ball;
float ball_speed = 1000;
sf::Vector2f ball_direction(1, 0);
sf::Vector2f ball_size(30, 30);

sf::Font default_font;
sf::Text debug_text;

void Initialize() {

	game_state = PLAYING;

	default_font.loadFromFile("../../resources/Inconsolata.otf");
	debug_text.setFont(default_font);
	debug_text.setCharacterSize(12);
	debug_text.setFillColor(sf::Color::White);

	sf::Color paddle_color = sf::Color::White;

	paddle_left.setFillColor(paddle_color);
	paddle_left.setSize(paddle_size);
	paddle_left.setOrigin(paddle_size.x/2, paddle_size.y/2);
	paddle_left.setPosition(paddle_offset, SCREEN_HEIGHT/2);

	paddle_right.setFillColor(paddle_color);
	paddle_right.setSize(paddle_size);
	paddle_right.setOrigin(paddle_size.x/2, paddle_size.y/2);
	paddle_right.setPosition(SCREEN_WIDTH-paddle_offset, SCREEN_HEIGHT/2);

	ball.setFillColor(paddle_color);
	ball.setSize(ball_size);
	ball.setOrigin(ball_size.x/2, ball_size.y/2);
	ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

}

void Update(float delta_time) {

	std::stringstream debug_stream;
	debug_stream << game_state;
	debug_text.setString(debug_stream.str());

	/////////////
	// PAUSING //
	/////////////

	if (input_bits & IRETURN) {
		if (game_state == PLAYING) {
			game_state = PAUSED;
		} else if (game_state == PAUSED) {
			game_state = PLAYING;
		}
		input_bits &= ~IRETURN;
	}

	if (game_state == PLAYING) {

		////////////////////////////
		// PLAYER PADDLE MOVEMENT //
		////////////////////////////

		sf::Vector2f paddle_vector;

		if (input_bits & IBACKWARDS) {
			paddle_vector.y = paddle_speed * delta_time;
		} else if (input_bits & IFORWARDS) {
			paddle_vector.y = -paddle_speed * delta_time;
		}

		paddle_right.move(paddle_vector);

		///////////////////////////
		// ENEMY PADDLE MOVEMENT //
		///////////////////////////

		// @Feature: Make this inconsistent
		sf::Vector2f ball_pos = ball.getPosition();
		sf::Vector2f left_pos = paddle_left.getPosition();

		if (ball_pos.y - left_pos.y < 0) {

			paddle_left.move(0, -paddle_speed * delta_time);

		} else {

			paddle_left.move(0, paddle_speed * delta_time);

		}

		///////////////////
		// BALL MOVEMENT //
		///////////////////

		sf::Vector2f next_ball_pos = ball.getPosition() + (ball_speed * delta_time * ball_direction);

		// PADDLE COLLISION
		// @Bug: Check if behind paddle
		if (next_ball_pos.x + ball_size.x / 2 > SCREEN_WIDTH - paddle_offset) {

			if (next_ball_pos.y < paddle_right.getPosition().y + paddle_size.y / 2 &&
				next_ball_pos.y > paddle_right.getPosition().y - paddle_size.y / 2) {

				ball_direction.x = -ball_direction.x;

			}

		} else if (next_ball_pos.x - ball_size.x / 2 < paddle_offset) {

			if (next_ball_pos.y < paddle_left.getPosition().y + paddle_size.y / 2 &&
				next_ball_pos.y > paddle_left.getPosition().y - paddle_size.y / 2) {

				ball_direction.x = -ball_direction.x;

			}

		}

		// WALL COLLISION
		if (next_ball_pos.y + ball_size.y / 2 > SCREEN_HEIGHT || next_ball_pos.y - ball_size.y / 2 < 0) {

			ball_direction.y = -ball_direction.y;

		}

		// OUT OF BOUNDS CHECK
		if (next_ball_pos.x > SCREEN_WIDTH) {

			next_ball_pos.x = SCREEN_WIDTH / 2;
			next_ball_pos.y = SCREEN_HEIGHT / 2;

		}

		if (next_ball_pos.y < 0) {

			next_ball_pos.x = SCREEN_WIDTH / 2;
			next_ball_pos.y = SCREEN_HEIGHT / 2;

		}

		ball.setPosition(next_ball_pos);

	}

}

void Draw(sf::RenderWindow * window) {

	window->draw(paddle_left);
	window->draw(paddle_right);
	window->draw(ball);
	window->draw(debug_text);

}
