#include <sstream>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "settings.hpp"

GameState game_state;
ControlType control_type;

float game_time;

sf::RectangleShape paddle;
sf::Vector2f paddle_size(30, 150);
float paddle_speed = 1000;
float paddle_offset = 50;
uint8_t lpp_size = 50;

sf::RectangleShape ball;
float ball_speed = 700;
float next_ball_speed;
sf::Vector2f ball_direction(1, 0.5);
sf::Vector2f ball_size(30, 30);

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

	paddle.setFillColor(paddle_color);
	paddle.setSize(paddle_size);
	paddle.setOrigin(paddle_size.x/2, paddle_size.y/2);
	paddle.setPosition(SCREEN_WIDTH-paddle_offset, SCREEN_HEIGHT/2);

	ball.setFillColor(paddle_color);
	ball.setSize(ball_size);
	ball.setOrigin(ball_size.x/2, ball_size.y/2);
	ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	cursor.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	cursor.setRadius(4);

}

void Update(float delta_time, sf::RenderWindow * window) {

	if (game_state == GAME_OVER) {
	
		if (input_bits & ILEFTCLICK) {
		
			ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			ball_direction = sf::Vector2f(1, 0.5);

			game_time = 0;
			ball_speed = 1000;

			game_state = PLAYING;

		}

	}

	if (game_state != GAME_OVER && game_state != PAUSED)
		game_time += delta_time;

	if (game_time > 100)
		next_ball_speed = 2500;
	else if (game_time > 75)
		next_ball_speed = 2000;
	else  if (game_time > 50)
		next_ball_speed = 1800;
	else if (game_time > 30)
		next_ball_speed = 1500;
	else if (game_time > 15)
		next_ball_speed = 1200;
	else if (game_time > 7)
		next_ball_speed = 1000;
	else
		next_ball_speed = 800;

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

		//last_paddle_pos = paddle.getPosition();
		paddle.move(0, mouse_vector.y / game_settings.sensitivity);

		///////////////////
		// BALL MOVEMENT //
		///////////////////

		sf::Vector2f next_ball_pos = ball.getPosition() + (ball_speed * delta_time * ball_direction);

		// PADDLE COLLISION
		if (next_ball_pos.x + ball_size.x / 2 > SCREEN_WIDTH - paddle_offset - paddle_size.x / 2 && 
			next_ball_pos.x - ball_size.x / 2 < SCREEN_WIDTH - paddle_offset - paddle_size.x / 2) {

			if (next_ball_pos.y - ball_size.y / 2 < paddle.getPosition().y + paddle_size.y / 2 &&
				next_ball_pos.y + ball_size.y / 2 > paddle.getPosition().y - paddle_size.y / 2) {

				ball_direction.x += 0.1;
				ball_direction.x = -ball_direction.x;

				if (ball_direction.y < 0.05 && ball_direction.y > -0.05) {
					
					ball_direction.y *= 200;

				}

				if (ball_direction.y < 0) {
				
					ball_direction.y *= (((abs(paddle.getPosition().y + paddle_size.y / 2) - next_ball_pos.y) * 2) / paddle_size.y - 0.2);

				} else if (ball_direction.y > 0) {
				
					ball_direction.y *= -(((abs(paddle.getPosition().y - paddle_size.y / 2) - next_ball_pos.y) * 2) / paddle_size.y + 0.2);

				}

				ball_speed = next_ball_speed;

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

			game_state = GAME_OVER;

		}

		ball.setPosition(next_ball_pos);

	}

	sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);

}

void Draw(sf::RenderWindow * window) {

	window->draw(paddle);
	window->draw(ball);
	window->draw(timer_text);
	if (game_settings.cursor_visible)
		window->draw(cursor);

}
