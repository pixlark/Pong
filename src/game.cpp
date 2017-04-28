#include <sstream>
#include <SFML/Graphics.hpp>

#include "input.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "settings.hpp"

GameState game_state;

float game_time;

Paddle paddle;
Ball ball;
SlowdownBall slowdown_ball;

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

	game_state = PLAYING;

	// Load game settings
	game_settings.cursor_visible = false;
	game_settings.sensitivity = 2;

	// Load font
	default_font = new sf::Font;
	default_font->loadFromFile("../../resources/Inconsolata.otf");

	// Timer text
	timer_text.setFont(*default_font);
	timer_text.setFillColor(sf::Color::White);

	// Paddle
	paddle.paddle = new sf::RectangleShape;
	paddle.size = sf::Vector2f(30, 150);
	paddle.paddle->setFillColor(sf::Color::White);
	paddle.paddle->setSize(paddle.size);
	paddle.paddle->setOrigin(paddle.size.x/2, paddle.size.y/2);
	paddle.paddle->setPosition(SCREEN_WIDTH-paddle.offset, SCREEN_HEIGHT/2);

	// Ball
	ball.direction = sf::Vector2f(1, 0.25);
	ball.size = sf::Vector2f(30, 30);
	ball.ball = new sf::RectangleShape;
	ball.ball->setFillColor(sf::Color::White);
	ball.ball->setSize(ball.size);
	ball.ball->setOrigin(ball.size.x/2, ball.size.y/2);
	ball.ball->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	ball.speed = ball.base_speed;
	ball.ball_state = REGULAR;

	// Slowdown powerup
	slowdown_ball.shape = new sf::CircleShape;
	slowdown_ball.shape->setFillColor(sf::Color::Green);
	slowdown_ball.shape->setRadius(45);
	slowdown_ball.shape->setOrigin(slowdown_ball.shape->getRadius(), slowdown_ball.shape->getRadius());
	slowdown_ball.shape->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	slowdown_ball.enabled = true;

	// Cursor
	cursor.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	cursor.setRadius(4);

}

bool IsBallCollidingWithPaddle(Paddle * paddle, sf::Vector2f ball_pos, sf::Vector2f ball_size) {

	if (ball_pos.x + ball.size.x / 2 > SCREEN_WIDTH - paddle->offset - paddle->size.x / 2 &&
		ball_pos.x - ball.size.x / 2 < SCREEN_WIDTH - paddle->offset - paddle->size.x / 2) {

		if (ball_pos.y - ball.size.y / 2 < paddle->paddle->getPosition().y + paddle->size.y / 2 &&
			ball_pos.y + ball.size.y / 2 > paddle->paddle->getPosition().y - paddle->size.y / 2) {

			return true;

		}

	}

	return false;

}

// @Refactor: Ugly function
// Returns true if there is a paddle collision
bool UpdateBallDirection(sf::Vector2f * ball_direction, sf::Vector2f ball_pos, const Ball * ball) {

	bool paddle_collision = false;

	// Paddle collision
	if (IsBallCollidingWithPaddle(&paddle, ball_pos, ball->size)) {

		paddle_collision = true;

		ball_direction->x += 0.1;
		ball_direction->x = -ball_direction->x;

		if (ball_direction->y < 0.05 && ball_direction->y > -0.05) {

			ball_direction->y *= 200;

		}

		if (ball_direction->y < 0) {

			ball_direction->y *= (((abs(paddle.paddle->getPosition().y + paddle.size.y / 2) - ball_pos.y) * 2) / paddle.size.y - 0.2);

		} else if (ball_direction->y > 0) {

			ball_direction->y *= -(((abs(paddle.paddle->getPosition().y - paddle.size.y / 2) - ball_pos.y) * 2) / paddle.size.y + 0.2);

		}

	}

	*ball_direction = NormalizeVector(*ball_direction);

	// Wall collision
	if (ball_pos.y + ball->size.y / 2 > SCREEN_HEIGHT || ball_pos.y - ball->size.y / 2 < 0) {

		if (ball_direction->x < 0.2 && ball_direction->x > 0) {
			ball_direction->x += 0.2;
		} else if (ball_direction->x > -0.2 && ball_direction->x < 0) {
			ball_direction->x += -0.2;
		} else if (ball_direction->x == 0) {
			ball_direction->x += -0.2;
		}

		ball_direction->y = -ball_direction->y;

	}

	if (ball_pos.x - ball->size.x / 2 < 0) {

		ball_direction->x = -ball_direction->x;

	}

	return paddle_collision;

}

float GetBallSpeed(float game_time) {

	if (game_time > 100)
		return 1800;
	else if (game_time > 75)
		return 1300;
	else  if (game_time > 50)
		return 1100;
	else if (game_time > 30)
		return 800;
	else if (game_time > 15)
		return 500;
	else if (game_time > 7)
		return 300;
	else
		return 0;

}

void Update(float delta_time, sf::RenderWindow * window) {

	// Update the mouse/cursor
	sf::Vector2f mouse_vector = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window)) - sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	cursor.move(mouse_vector.x / game_settings.sensitivity, mouse_vector.y / game_settings.sensitivity);

	if (game_state == PLAYING) {

		// Pause on pause button
		if (input_bits & IRETURN) {
			game_state = PAUSED;
			input_bits &= ~IRETURN;
		}

		// Update game time
		game_time += delta_time;
		ball.next_speed = ball.base_speed + GetBallSpeed(game_time);

		// Update timer text
		char timer_string[8];
		sprintf_s(timer_string, "%.2f", game_time);
		timer_text.setString(timer_string);

		// Move the paddle
		paddle.paddle->move(0, mouse_vector.y / game_settings.sensitivity);

		// Move the ball
		sf::Vector2f next_ball_pos = ball.ball->getPosition() + (ball.speed * delta_time * ball.direction);
		if(UpdateBallDirection(&ball.direction, next_ball_pos, &ball))
			ball.speed = ball.next_speed;
		next_ball_pos = ball.ball->getPosition() + (ball.speed * delta_time * ball.direction);
		ball.ball->setPosition(next_ball_pos);

		// Slowdown powerup collision
		if (slowdown_ball.enabled == true &&
			sqrt(pow(slowdown_ball.shape->getPosition().x - next_ball_pos.x, 2) + pow(slowdown_ball.shape->getPosition().y - next_ball_pos.y, 2)) 
				< slowdown_ball.shape->getRadius() * 2) {
		
			ball.ball_state = SLOWDOWN;
			ball.ball->setFillColor(sf::Color::Green);

		}

		// Check for loss
		if (next_ball_pos.x > SCREEN_WIDTH || next_ball_pos.x < 0) game_state = GAME_OVER;

	}

	if (game_state == GAME_OVER) {

		if (input_bits & ILEFTCLICK) {

			ball.ball->setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			ball.direction = sf::Vector2f(1, 0.5);

			game_time = 0;
			ball.speed = 1000;

			game_state = PLAYING;

		}

	}

	if (game_state == PAUSED) {
	
		if (input_bits & IRETURN) {
			game_state = PLAYING;
			input_bits &= ~IRETURN;
		}

	}

	// Move mouse back to center
	sf::Mouse::setPosition(sf::Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), *window);

}

void Draw(sf::RenderWindow * window) {

	window->draw(*paddle.paddle);
	if (slowdown_ball.enabled) window->draw(*slowdown_ball.shape);
	window->draw(*ball.ball);
	window->draw(timer_text);
	if (game_settings.cursor_visible)
		window->draw(cursor);

}
