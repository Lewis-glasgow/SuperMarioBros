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
	deathBuffer.loadFromFile("assets/audio/death.wav");
	DeathSound.setBuffer(deathBuffer);
}

float interpolate(float from, float to, float time) {
	return from + (to - from) * time;
	//A + (B - A) * t
}

void Player::Animate(string animName) {
	if (smallMario && currentAnim != "mushroom" && currentAnim != "shrink" && currentAnim != "dead" && currentAnim != "fireBall") {
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
				if (frameCounter.getElapsedTime().asSeconds() > movementAnimSpeed) {
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
		if (currentAnim != "mushroom" && currentAnim != "shrink" && currentAnim != "dead" && currentAnim != "fireBall") {
			if (isOnFloor || MoveLockState) {
				if (animName == "idle") {
					if (fire) {
						xFrame = 179;
						yFrame = 121;
					}
					else {
						xFrame = 6;
						yFrame = 0;
					}
					currentAnim = animName;
				}

				else if (animName == "run") {
					if (currentAnim != animName) {
						if (fire) {
							xFrame = 101;
							yFrame = 121;
						}
						else {
							xFrame = 3;
							yFrame = 0;
						}
						currentAnim = animName;
						frameCounter.restart();
					}
					if (frameCounter.getElapsedTime().asSeconds() > movementAnimSpeed) {
						if (fire) {
							xFrame += 25;
						}
						else {
							xFrame += 1;
						}
						frameCounter.restart();
					}
					if (fire) {
						if (xFrame > 151) {
							xFrame = 101;
						}
					}
					else {
						if (xFrame >= 6) {
							xFrame = 3;
						}
					}
				}
				else if (animName == "turn") {
					if (fire) {
						xFrame = 51;
						yFrame = 121;
					}
					else {
						xFrame = 2;
						yFrame = 0;
					}
					currentAnim = animName;
				}
			}
			else {
				if (fire) {
					xFrame = 26;
					yFrame = 121;
				}
				else {
					xFrame = 1;
					yFrame = 0;
				}
				currentAnim = "air";
			}
		}
	}
	if (animName == "mushroom" && currentAnim != "shrink") {
		fire = false;
		if (currentAnim != "mushroom") {
			frameCounter.restart();
			xFrame = 1;
			mushFrame = 0;
			yFrame = 0;
			currentAnim = "mushroom";
		}
		if (frameCounter.getElapsedTime().asSeconds() >= 0.1) {
			frameCounter.restart();

			if (mushFrame == 0) {
				smallMario = false;
				sprite.move(0, -16);
			}
			else if (mushFrame == 1) {
				smallMario = true;
				sprite.move(0, 16);
			}
			else if (mushFrame == 2) {
				smallMario = false;
				sprite.move(0, -16);
			}
			else if (mushFrame == 3) {
				smallMario = true;
				sprite.move(0, 16);
			}
			else if (mushFrame == 4) {
				smallMario = false;
				sprite.move(0, -10);
				currentAnim = "";
				MoveLockState = false;
			}
			mushFrame += 1;
		}
	}
	if (animName == "shrink") {
		CanBeHit = false;
		if (currentAnim != "shrink") {
			frameCounter.restart();
			xFrame = 1;
			yFrame = 0;
			mushFrame = 0;
			currentAnim = "shrink";
		}
		if (frameCounter.getElapsedTime().asSeconds() >= 0.1) {
			frameCounter.restart();
			if (mushFrame == 0) {
				smallMario = true;
				sprite.move(0, 16);
			}
			else if (mushFrame == 1) {
				smallMario = false;
				sprite.move(0, -16);
			}
			else if (mushFrame == 2) {
				smallMario = true;
				sprite.move(0, 16);
			}
			else if (mushFrame == 3) {
				smallMario = false;
				sprite.move(0, -16);
			}
			else if (mushFrame == 4) {
				sprite.move(0, 10);
				smallMario = true;
				MoveLockState = false;
				hitTimer.restart();
				currentAnim = "";
			}
			mushFrame += 1;
		}
	}
	if (animName == "dead") {
		fire = false;
		currentAnim = "dead";
		sprite.setTextureRect(sf::IntRect(0, 17, 17, 17));

	}
	else if (animName == "fireBall") {
		if (currentAnim != animName) {
			currentAnim = animName;
			frameCounter.restart();
		}
		xFrame = 76;
		yFrame = 121;
		if (frameCounter.getElapsedTime().asSeconds() > 0.1) {
			currentAnim = "";
		}
	}
	else if (animName == "pole") {
		sprite.setScale(-scaleFactor, scaleFactor);
		if (smallMario) {
			sprite.setTextureRect(sf::IntRect(59, 29, 17, 17));
		}
		else if (fire) {
			sprite.setTextureRect(sf::IntRect(26, 155, 17, 33));
		}
		else {
			sprite.setTextureRect(sf::IntRect(26, 88, 17, 33));
		}
	}
	else {
		// 30 is gap between sprite
		if (smallMario) {
			sprite.setTextureRect(sf::IntRect(29 + (30 * xFrame), 30 * yFrame, 17, 17));
		}
		else {
			if (fire && !dead) {

				sprite.setTextureRect(sf::IntRect(xFrame, yFrame, 17, 33));
			}
			else {
				sprite.setTextureRect(sf::IntRect(30 * xFrame, 52 + (30 * yFrame), 17, 33));
			}
		}
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
		hitbox.setPosition(sprite.getPosition() + sf::Vector2f(0, 2.f));
		hitbox.setSize(sf::Vector2f(13, 30));

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
					grav = -1;
				}
			}
		}
	}
}

void Player::Logic(vector<sf::Sprite> tiles, bool windowFocus) {
	vel = sf::Vector2f();
	action = "";
	if (currentAnim == "mushroom") {
		Animate("mushroom");
	}
	else if (currentAnim == "shrink") {
		Animate("shrink");
	}
	else if (dead) {
		Animate("dead");
		fire = false;
		smallMario = true;
	}
	else if (currentAnim == "fireBall") {
		Animate("fireBall");
	}
	if (!CanBeHit) {
		sprite.setColor(sf::Color(255, 255, 255, 100));
	}
	else {
		sprite.setColor(sf::Color(255, 255, 255, 255));
	}
	if (windowFocus && !MoveLockState && !dead) {
		if (hitTimer.getElapsedTime().asSeconds() > 5 && currentAnim != "shrink") {
			CanBeHit = true;
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerNum == 0) || sf::Joystick::getAxisPosition(playerNum, sf::Joystick::X) > axisThreshold) {
			vel.x = current_speed;
			sprite.setScale(sf::Vector2f(-scaleFactor, scaleFactor));
			if (current_speed > 0) {
				Animate("run");
			}
			else {
				Animate("turn");
			}

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B) && playerNum == 0) || sf::Joystick::isButtonPressed(playerNum, 2)) {
				current_speed = interpolate(current_speed, maxRunSpeed, groundAcc / 10);
				movementAnimSpeed = interpolate(movementAnimSpeed, 0.06, 0.1);
			}
			else {
				fireOnce = true;
				current_speed = interpolate(current_speed, maxSpeed, groundAcc / 10);
				movementAnimSpeed = interpolate(movementAnimSpeed, 0.1, 0.1);
			}
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

			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B) && playerNum == 0) || sf::Joystick::isButtonPressed(playerNum, 1)) {
				current_speed = interpolate(current_speed, -maxRunSpeed, groundAcc / 10);
				movementAnimSpeed = interpolate(movementAnimSpeed, 0.06, 0.1);
			}
			else {
				current_speed = interpolate(current_speed, -maxSpeed, groundAcc / 10);
				movementAnimSpeed = interpolate(movementAnimSpeed, 0.1, 0.1);
			}
		}
		else {
			Animate("idle");
			vel.x = current_speed;
			current_speed = interpolate(current_speed, 0, groundAcc / 2);
		}
	}
	else {
		current_speed = 0;
		if (!dead){
			grav = 0;
		}
		else {
			hitbox.setPosition(0, 0);
		}
	}
	if (isOnFloor) {
		if (!dead) {
			grav = .2f;
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && playerNum == 0)|| sf::Joystick::isButtonPressed(playerNum, 1)) && windowFocus && !dead && !MoveLockState) {
				if (jumpOnce) {
					jumpSound.play();
					grav = -jumpForce;
					jumpOnce = false;
				}
			}
			else {
				jumpOnce = true;
			}
		}
	}
	else {
		if (dead) {
			grav += grav_strength/2;
		}
		else {
			grav += grav_strength;
		}
	}

	if (grav < 0 && !dead) {
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
	if (dead) {
		if (!deadOnceSound) {
			grav = -4;
			DeathSound.play();
			deathTimer.restart();
			deadOnceSound = true;
		}
		if (deathTimer.getElapsedTime().asSeconds() > 3) {
			switchScreen = true;
		}
	}

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::B) && playerNum == 0) || sf::Joystick::isButtonPressed(playerNum, 2)) {
		if (fireOnce) {
			if (fire) {
				Animate("fireBall");
				action = "fire";
			}
			fireOnce = false;
		}
	}
	else {
		fireOnce = true;
	}

	vel.y += grav;
	//hitbox.setPosition(sprite.getPosition());
	if (!MoveLockState && finished == 0) {
		isOnFloor = false;
		if (!dead) {
			sprite.move(sf::Vector2f(vel.x, 0));
			Collision(tiles, sf::Vector2f(vel.x, 0));
			sprite.move(sf::Vector2f(0, vel.y));
			Collision(tiles, sf::Vector2f(0, vel.y));
		}
		else {
			sprite.move(sf::Vector2f(0, vel.y));
		}
	}


	if (sprite.getPosition().x > Camera.getCenter().x-25 && !camLockState && vel.x > 0) {
		Camera.move(vel.x, 0);
	}
	areaBounds.setPosition(Camera.getCenter()-areaBounds.getSize()/2.f);

}

sf::FloatRect Player::getRect() {
	return sprite.getGlobalBounds();
}