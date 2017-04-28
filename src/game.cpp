#include <sstream>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "settings.hpp"

GameState game_state;
ControlType control_type;

float game_time;

Paddle paddle;
Ball ball;

sf::Font * default_font;
sf::Text timer_text;

sf::CircleShape cursor;

sf::Vector2f NormalizeVector(sf::Vector2f start_vector) {

	float length = sqrt((start_vector.x * start_vector.x) + (start_vector.y * start_vector.y));
	if (length != 0) {
		return sf::Vector2f(start_vector.x / length, start_vector.y / length);
	}
	return start_vector;

}

void Initialize() {

	game_settings.cursor_visible = false;
	game_settings.sensitivity = 2;

	default_font = new sf::Font;
	default_font->loadFromFile("../../resources/Inconsolata.otf");

	timer_text.setFont(*default_font);
	timer_text.setFillColor(sf::Color::White);

	game_state = PLAYING;
	control_type = MOUSE;

	sf::Color paddle_color = sf::Color::White;

	//@Refactor: There's a whole lotta initialization going on here
	paddle.paddle = new sf::RectangleShape;
	paddle.size = sf::Vector2f(30, 150);
	paddle.paddle->setFillColor(paddle_color);
	paddle.paddle->setSize(paddle.size);
	paddle.paddle->setOrigin(paddle.size.x/2, paddle.size.y/2);
	paddle.paddle->setPosition(SCREEN_WIDTH-paddle.offset, SCREEN_HEIGHT/2);

	//@Refactor: See above
	ball.direction = sf::Vector2f(1, 0.5);
	ball.size = sf::Vector2f(30, 30);
	ball.ball = new sf::RectangleShape;
	ball.ball->setFillColor(paddle_color);
	ball.ball->setSize(ball.size);
	ball.ball->setOrigin(ball.size.x/2, ball.size.y/2);
	ball.ball->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	cursor.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	cursor.setRadius(4);

}

void Update(float delta_time, sf::RenderWindow * window) {

	if (game_state == GAME_OVER) {
	
		if (input_bits & ILEFTCLICK) {
		
			ball.ball->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			ball.direction = sf::Vector2f(1, 0.5);

			game_time = 0;
			ball.speed = 1000;

			game_state = PLAYING;

		}

	}

	if (game_state != GAME_OVER && game_state != PAUSED)
		game_time += delta_time;

	if (game_time > 100)
		ball.next_speed = 2500;
	else if (game_time > 75)
		ball.next_speed = 2000;
	else  if (game_time > 50)
		ball.next_speed = 1800;
	else if (game_time > 30)
		ball.next_speed = 1500;
	else if (game_time > 15)
		ball.next_speed = 1200;
	else if (game_time > 7)
		ball.next_speed = 1000;
	else
		ball.next_speed = 800;

	////////////////
	// TIMER TEXT //
	////////////////
	char timer_string[8];
	sprintf_s(timer_string, "%.2f", game_time);
	timer_text.setString(timer_string);

	////////////
	// CURSOR //
	////////////

	sf::Vector2f mouse_vector = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)) - sf::Vector2f(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	cursor.move(mouse_vector.x / game_settings.sensitivity, mouse_vector.y / game_settings.sensitivity);

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

		paddle.paddle->move(0, mouse_vector.y / game_settings.sensitivity);

		///////////////////
		// BALL MOVEMENT //
		///////////////////

		sf::Vector2f next_ball_pos = ball.ball->getPosition() + (ball.speed * delta_time * ball.direction);

		// PADDLE COLLISION
		if (next_ball_pos.x + ball.size.x / 2 > SCREEN_WIDTH - paddle.offset - paddle.size.x / 2 && 
			next_ball_pos.x - ball.size.x / 2 < SCREEN_WIDTH - paddle.offset - paddle.size.x / 2) {

			if (next_ball_pos.y - ball.size.y / 2 < paddle.paddle->getPosition().y + paddle.size.y / 2 &&
				next_ball_pos.y + ball.size.y / 2 > paddle.paddle->getPosition().y - paddle.size.y / 2) {

				ball.direction.x += 0.1;
				ball.direction.x = -ball.direction.x;

				if (ball.direction.y < 0.05 && ball.direction.y > -0.05) {
					
					ball.direction.y *= 200;

				}

				if (ball.direction.y < 0) {
				
					ball.direction.y *= (((abs(paddle.paddle->getPosition().y + paddle.size.y / 2) - next_ball_pos.y) * 2) / paddle.size.y - 0.2);

				} else if (ball.direction.y > 0) {
				
					ball.direction.y *= -(((abs(paddle.paddle->getPosition().y - paddle.size.y / 2) - next_ball_pos.y) * 2) / paddle.size.y + 0.2);

				}

				ball.speed = ball.next_speed;

			}

		}

		ball.direction = NormalizeVector(ball.direction);

		// WALL COLLISION
		if (next_ball_pos.y + ball.size.y / 2 > SCREEN_HEIGHT || next_ball_pos.y - ball.size.y / 2 < 0) {

			if (ball.direction.x < 0.2 && ball.direction.x > 0) {
				ball.direction.x += 0.2;
			} else if (ball.direction.x > -0.2 && ball.direction.x < 0) {
				ball.direction.x += -0.2;
			} else if (ball.direction.x == 0) {
				ball.direction.x += -0.2;
			}

			ball.direction.y = -ball.direction.y;

		}

		if (next_ball_pos.x - ball.size.x / 2 < 0) {
		
			ball.direction.x = -ball.direction.x;

		}

		next_ball_pos = ball.ball->getPosition() + (ball.speed * delta_time * ball.direction);

		// OUT OF BOUNDS CHECK
		if (next_ball_pos.x > SCREEN_WIDTH || next_ball_pos.x < 0) {

			game_state = GAME_OVER;

		}

		ball.ball->setPosition(next_ball_pos);

	}

	sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);

}

void Draw(sf::RenderWindow * window) {

	window->draw(*paddle.paddle);
	window->draw(*ball.ball);
	window->draw(timer_text);
	if (game_settings.cursor_visible)
		window->draw(cursor);

}
