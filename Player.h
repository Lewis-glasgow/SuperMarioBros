#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

using namespace std;
class Player
{
private:
	sf::Texture texture;
	float scaleFactor = 1.f;

	float maxSpeed = 2.f;
	float maxRunSpeed = 2.8f;
	float grav_strength = 0.3f;
	float jumpForce = 6.4f;
	float groundAcc = 0.2f;

	bool jumpOnce = false;
	float current_speed = 0;

	string currentAnim;
	float xFrame;
	float yFrame;
	int mushFrame;
	sf::Clock frameCounter;
	float movementAnimSpeed = 0.1;

	sf::SoundBuffer jumpBuffer;
	sf::Sound jumpSound;
	sf::SoundBuffer deathBuffer;
	sf::Sound DeathSound;

public:
	sf::RectangleShape areaBounds;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::View Camera;

	string action;
	float grav = 0;
	bool isOnFloor = false;
	bool camLockState = false;
	bool MoveLockState = false;
	float axisThreshold = 50.f;

	sf::Clock finishedWaitTimer;
	sf::Clock hitTimer;
	bool CanBeHit = true;
	bool dead;
	bool deadOnceSound;
	bool switchScreen = false;
	sf::Clock deathTimer;

	int coins = 0;
	int score = 0;

	int finished = 0;
	bool finishedOnce = true;
	sf::Vector2f finalCamPos;

	bool fire = false;
	bool smallMario = true;
	bool fireOnce = true;
	int playerNum;
	Player(sf::RenderWindow& window, string SpriteSheet, int num);
	void Animate(string animName);
	void Collision(vector<sf::Sprite>& tiles, sf::Vector2f vel);
	sf::Vector2f vel;
	void Logic(vector<sf::Sprite> tiles, bool windowFocus);
	//void draw(sf::RenderWindow& window);
	sf::FloatRect getRect();
};

