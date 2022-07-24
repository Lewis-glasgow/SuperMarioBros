#include "Player.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

Player::Player(sf::RenderWindow& window, string SpriteSheet, int num) {
	texture.loadFromFile(SpriteSheet);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(29, 0, 17, 17));
	sprite.setOrigin(sf::Vector2f(8.5, 8.5));
	sprite.setScale(sf::Vector2f(-scaleFactor, scaleFactor));
	sprite.setPosition(sf::Vector2f(256 / 5, 224-32-8.5));

	Camera.reset(sf::FloatRect(0, 0, 256, 224));

	hitbox.setSize(sf::Vector2f(13, 17));
	hitbox.setOrigin(sf::Vector2f(6.5, 8.5));

	areaBounds.setSize(sf::Vector2f( 256, 224));
	//areaBounds.setOrigin(areaBounds.getSize() / 2.f);

	playerNum = num;

	jumpBuffer.loadFromFile("assets/audio/jump.wav");
	jumpSound.setBuffer(jumpBuffer);
}

float interpolate(float from, float to, float time) {
	return from + (to - from) * time;
	//A + (B - A) * t
}

void Player::Animate(string animName) {
	if (smallMario) {
		if (isOnFloor || MoveLockState) {
			if (animName == "idle") {
				xFrame = 5;
				yFrame = 0;
				currentAnim = animName;
			}

			else if (animName == "run") {
				if (currentAnim != animName) {
					xFrame = 2;
					yFrame = 0;
					currentAnim = animName;
					frameCounter.restart();
				}
				if (frameCounter.getElapsedTime().asSeconds() > 0.1) {
					xFrame += 1;
					frameCounter.restart();
				}
				if (xFrame >= 5) {
					xFrame = 2;
				}
			}
			else if (animName == "turn") {
				xFrame = 1;
				yFrame = 0;
				currentAnim = animName;
			}
		}
		else {
			xFrame = 0;
			yFrame = 0;
			currentAnim = "air";
		}
	}
	else {
		if (isOnFloor || MoveLockState) {
			if (animName == "idle") {
				xFrame = 6;
				yFrame = 0;
				currentAnim = animName;
			}

			else if (animName == "run") {
				if (currentAnim != animName) {
					xFrame = 3;
					yFrame = 0;
					currentAnim = animName;
					frameCounter.restart();
				}
				if (frameCounter.getElapsedTime().asSeconds() > 0.1) {
					xFrame += 1;
					frameCounter.restart();
				}
				if (xFrame >= 6) {
					xFrame = 3;
				}
			}
			else if (animName == "turn") {
				xFrame = 2;
				yFrame = 0;
				currentAnim = animName;
			}
		}
		else {
			xFrame = 1;
			yFrame = 0;
			currentAnim = "air";
		}
	}
	// 30 is gap between sprite
	if (smallMario) {
		sprite.setTextureRect(sf::IntRect(29 + (30 * xFrame), 30 * yFrame, 17, 17));
	}
	else {
		sprite.setTextureRect(sf::IntRect(30*xFrame, 52+(30*yFrame), 17, 33));
	}
}

void Player::Collision(vector<sf::Sprite>& tiles, sf::Vector2f vel) {
	if (smallMario) {
		sprite.setOrigin(sf::Vector2f(8.5, 8.5));
		hitbox.setPosition(sprite.getPosition() - sf::Vector2f(0, 1.f));
		hitbox.setSize(sf::Vector2f(13, 17));
		hitbox.setOrigin(sf::Vector2f(6.5, 8.5));

		for (int tile = 0; tile < tiles.size(); tile++) {
			if (hitbox.getGlobalBounds().intersects(tiles[tile].getGlobalBounds())) {
				if (vel.x > 0) {
					sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left - 6.5, sprite.getPosition().y));
					vel.x = 0;
				}
				if (vel.x < 0) {
					sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left + tiles[tile].getGlobalBounds().width + 6.5, sprite.getPosition().y));
					vel.x = 0;
				}

				if (vel.y > 0) {
					sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top - 7.5));
					isOnFloor = true;
				}
				if (vel.y < 0) {
					sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top + tiles[tile].getGlobalBounds().height + 9.5));
					grav = 1;
				}
			}
		}
	}
	else {
		sprite.setOrigin(sf::Vector2f(8.5, 16.5));
		hitbox.setOrigin(sf::Vector2f(6.5, 16.f));
		hitbox.setPosition(sprite.getPosition() + sf::Vector2f(0, -1.f));
		hitbox.setSize(sf::Vector2f(13, 33));

		for (int tile = 0; tile < tiles.size(); tile++) {
			if (hitbox.getGlobalBounds().intersects(tiles[tile].getGlobalBounds())) {
				if (vel.x > 0) {
					sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left - 6.5, sprite.getPosition().y));
				}
				if (vel.x < 0) {
					sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left + tiles[tile].getGlobalBounds().width + 6.5, sprite.getPosition().y));
				}

				if (vel.y > 0) {
					sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top - 16));
					isOnFloor = true;
				}
				if (vel.y < 0) {
					sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top + tiles[tile].getGlobalBounds().height + 17.5));
					grav = 1;
				}
			}
		}
	}
}

void Player::Logic(vector<sf::Sprite> tiles, bool windowFocus) {
	vel = sf::Vector2f();
	if (windowFocus && !MoveLockState) {

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerNum == 0) || sf::Joystick::getAxisPosition(playerNum, sf::Joystick::X) > axisThreshold) {
			vel.x = current_speed;
			sprite.setScale(sf::Vector2f(-scaleFactor, scaleFactor));
			if (current_speed > 0) {
				Animate("run");
			}
			else {
				Animate("turn");
			}
			current_speed = interpolate(current_speed, maxSpeed, groundAcc / 10);
		}
		else if (((sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerNum == 0)|| sf::Joystick::getAxisPosition(playerNum, sf::Joystick::X) < -axisThreshold) && Camera.getCenter().x - (256 / 2) + 25 < sprite.getPosition().x) {
			vel.x = current_speed;
			sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
			if (current_speed < 0) {
				Animate("run");
			}
			else {
				Animate("turn");
			}
			current_speed = interpolate(current_speed, -maxSpeed, groundAcc / 10);
		}
		else {
			Animate("idle");
			vel.x = current_speed;
			current_speed = interpolate(current_speed, 0, groundAcc / 2);
		}
	}
	else {
		current_speed = 0;
		grav = 0;
	}
	if (isOnFloor) {
		grav = .2f;
		if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && playerNum == 0)|| sf::Joystick::isButtonPressed(playerNum, 1)) && windowFocus) {
			jumpSound.play();
			grav = -jumpForce;
		}
	}
	else {
		grav += grav_strength;
	}
	vel.y += grav;

	if (grav < 0) {
		if (playerNum == 0) {
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !sf::Joystick::isButtonPressed(0, 1)) {
				grav = grav / 1.2;
			}
		}
		else {
			if (!sf::Joystick::isButtonPressed(playerNum, 1)) {
				grav = grav / 1.2;
			}
		}
	}

	if (!MoveLockState) {
		isOnFloor = false;
		sprite.move(sf::Vector2f(vel.x, 0));
		Collision(tiles, sf::Vector2f(vel.x, 0));
		sprite.move(sf::Vector2f(0, vel.y));
		Collision(tiles, sf::Vector2f(0, vel.y));
	}

	if (sprite.getPosition().x > Camera.getCenter().x && !camLockState && vel.x > 0) {
		Camera.move(vel.x, 0);
	}
	areaBounds.setPosition(Camera.getCenter()-areaBounds.getSize()/2.f);

}

sf::FloatRect Player::getRect() {
	return sprite.getGlobalBounds();
}