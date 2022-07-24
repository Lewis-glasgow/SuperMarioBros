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

	const float maxSpeed = 2.f;
	const float grav_strength = 0.3f;
	const float jumpForce = 6.2f;
	const float groundAcc = 0.2f;

	float current_speed = 0;
	float grav = 0;
	bool isOnFloor = false;

	string currentAnim;
	float xFrame;
	float yFrame;
	sf::Clock frameCounter;

	sf::SoundBuffer jumpBuffer;
	sf::Sound jumpSound;

public:
	sf::RectangleShape areaBounds;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::View Camera;
	int score = 0;
	bool camLockState = false;
	bool MoveLockState = false;
	float axisThreshold = 50.f;

	int playerNum;
	Player(sf::RenderWindow& window, string SpriteSheet, int num);
	bool smallMario = true;
	void Animate(string animName);
	void Collision(vector<sf::Sprite>& tiles, sf::Vector2f vel);
	sf::Vector2f vel;
	void Logic(vector<sf::Sprite> tiles, bool windowFocus);
	//void draw(sf::RenderWindow& window);
	sf::FloatRect getRect();
};

