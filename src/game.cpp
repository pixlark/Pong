#include <sstream>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"

GameState game_state;
ControlType control_type;

float game_time;

sf::RectangleShape paddle;
sf::Vector2f paddle_size(30, 200);
float paddle_speed = 1000;
float paddle_offset = 50;
uint8_t lpp_size = 50;

sf::RectangleShape ball;
float ball_speed = 1000;
sf::Vector2f ball_direction(1, 0);
sf::Vector2f ball_size(30, 30);

sf::CircleShape cursor;

sf::Vector2f NormalizeVector(sf::Vector2f start_vector) {

	float length = sqrt((start_vector.x * start_vector.x) + (start_vector.y * start_vector.y));
	if (length != 0) {
		return sf::Vector2f(start_vector.x / length, start_vector.y / length);
	}
	return start_vector;

}

void Initialize() {

	game_state = PLAYING;
	control_type = MOUSE;

	sf::Color paddle_color = sf::Color::White;

	paddle.setFillColor(paddle_color);
	paddle.setSize(paddle_size);
	paddle.setOrigin(paddle_size.x/2, paddle_size.y/2);
	paddle.setPosition(SCREEN_WIDTH-paddle_offset, SCREEN_HEIGHT/2);

	ball.setFillColor(paddle_color);
	ball.setSize(ball_size);
	ball.setOrigin(ball_size.x/2, ball_size.y/2);
	ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	cursor.setRadius(4);

}

void Update(float delta_time, sf::RenderWindow * window) {

	game_time += delta_time;
	ball_speed *= 1 + (game_time / 5000000);

	////////////
	// CURSOR //
	////////////

	sf::Vector2f mouse_vector = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)) - sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	cursor.move(mouse_vector);

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

		/////////////////////
		// PADDLE MOVEMENT //
		/////////////////////

		//last_paddle_pos = paddle.getPosition();
		paddle.move(0, mouse_vector.y / 2);

		///////////////////
		// BALL MOVEMENT //
		///////////////////

		sf::Vector2f next_ball_pos = ball.getPosition() + (ball_speed * delta_time * ball_direction);

		// PADDLE COLLISION
		if (next_ball_pos.x + ball_size.x / 2 > SCREEN_WIDTH - paddle_offset - paddle_size.x / 2 && 
			next_ball_pos.x - ball_size.x / 2 < SCREEN_WIDTH - paddle_offset - paddle_size.x / 2) {

			if (next_ball_pos.y < paddle.getPosition().y + paddle_size.y / 2 &&
				next_ball_pos.y > paddle.getPosition().y - paddle_size.y / 2) {

				ball_direction.x += 0.1;
				ball_direction.x = -ball_direction.x;

			}

		}

		ball_direction = NormalizeVector(ball_direction);

		// WALL COLLISION
		if (next_ball_pos.y + ball_size.y / 2 > SCREEN_HEIGHT || next_ball_pos.y - ball_size.y / 2 < 0) {

			if (ball_direction.x < 0.2 && ball_direction.x > 0) {
				ball_direction.x += 0.2;
			} else if (ball_direction.x > -0.2 && ball_direction.x < 0) {
				ball_direction.x += -0.2;
			} else if (ball_direction.x == 0) {
				ball_direction.x += -0.2;
			}

			ball_direction.y = -ball_direction.y;

		}

		if (next_ball_pos.x - ball_size.x / 2 < 0) {
		
			ball_direction.x = -ball_direction.x;

		}

		next_ball_pos = ball.getPosition() + (ball_speed * delta_time * ball_direction);

		// OUT OF BOUNDS CHECK
		if (next_ball_pos.x > SCREEN_WIDTH || next_ball_pos.x < 0) {

			next_ball_pos.x = SCREEN_WIDTH / 2;
			next_ball_pos.y = SCREEN_HEIGHT / 2;

			game_time = 0;
			ball_speed = 1000;

		}

		ball.setPosition(next_ball_pos);

	}

	sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);

}

void Draw(sf::RenderWindow * window) {

	window->draw(paddle);
	window->draw(ball);
	//window->draw(cursor);

}
