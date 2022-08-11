#pragma once
#include "TileEditor.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include <fstream>
using namespace std;

class Enemy {
public:
	sf::Sprite s;
	int type;
	bool debug;
	float speed = 0.5;
	int direction = -1;
	sf::Clock FrameCounter;
	int xframe, yframe;
	int activated = 0;
	float grav = 0;
	sf::Vector2f collisionOffset;
	bool moved = false;

	Enemy(sf::Texture& spritesheet, sf::Vector2f position, int t, bool d);
	bool Logic(Player& Mario, bool oneTime, vector<sf::Sprite>& Tiles, vector<Enemy>& Enemies, sf::Sound& gumbaDead);
	void collision(vector<sf::Sprite>& tiles, sf::Vector2f vel, Player& Mario, vector<Enemy>& Enemies);
};

class StaticTile {
public:
	sf::Sprite s;
	sf::Vector2f startPos;
	bool moved = false;
	sf::Clock TimeCount;
	StaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect);
	int logic(Player& player, sf::FloatRect PRect, vector<Enemy>& Enemies);
};


class coinTile {
public:
	sf::Sprite s;
	sf::Clock FrameCounter;
	int xFrame = 759;
	bool activated = false;
	int type = 0;
	bool moved;
	coinTile(sf::Texture& tileset, sf::Vector2f pos, int t);
	bool Logic(Player& Mario, sf::Sound& sound, vector<Enemy>& Enemies);
};

class pipeTransportTile {
public:
	sf::Vector2f toPos;
	sf::Vector2f cameraPos;
	sf::RectangleShape s;
	sf::Vector2f dir;
	bool lockCam = false;
	bool active = false;
	int timer = 0;
	pipeTransportTile(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f ToPos, sf::Vector2f CameraPos, sf::Vector2f dir);
	void Logic(Player& Mario, bool oneTimes, sf::Sound& Sound);
};

class pickupTile {
public:
	sf::Sprite sprite;
	bool activated = false;
	int type = 0;
	float direction = 1;
	int speed = 1;
	sf::Clock waitTime;
	sf::Vector2f startPos;
	pickupTile(sf::Vector2f pos, int t, sf::Texture& tileset);
	void collision(vector<sf::Sprite>& tiles, sf::Vector2f vel);
	bool Logic(Player& Mario, vector<sf::Sprite>& tiles, bool oneTime, sf::Sound& mushSound, sf::Sound& activatedSound);
};

class Particle {
public:
	sf::CircleShape s;
	sf::Vector2f velocity;
	Particle(sf::Vector2f position, sf::Vector2f vel, sf::Texture* TileSet, sf::IntRect TextRect);
	void Logic();
};

class FireBall {
public:
	int dir;
	float grav = 0;
	sf::Sprite s;
	sf::Clock deathTimer;
	sf::Clock frameCounter;
	FireBall(sf::Texture& spriteSheet, sf::Vector2f pos, int d);
	bool Logic(vector<Enemy>& Enemies, vector<sf::Sprite>& AllTiles, bool oneTime);
	bool collision(float velx, float vely, vector<sf::Sprite>& AllTiles);
};

class PoleTile {
public:
	int side = 0;
	sf::Sprite s;
	sf::Clock waitTimer;

	bool activated = false;
	PoleTile(sf::Texture& tileSet, sf::Vector2f pos, int e);
	void Logic(Player& Mario);
};

class Entity
{
public:
	sf::Color BGColor;
	sf::Vector2f CamPos;

	bool debugMode = false;
	vector<StaticTile> StaticTiles;
	vector<sf::Sprite> castleTiles;
	vector<coinTile> CoinTiles;
	vector<sf::Sprite> AllTiles;
	vector<sf::Sprite> backgroundTiles;
	vector<pipeTransportTile> transportTiles;
	vector<pickupTile> pickupTiles;
	vector<Enemy> Enemies;
	vector<sf::RectangleShape> deathTriggers;
	vector<Particle> particles;
	vector<FireBall> FireBalls;
	vector<PoleTile> Poles;

	sf::SoundBuffer coinCollectedBuffer;
	sf::Sound coinCollected;
	sf::SoundBuffer mushCollectedBuffer;
	sf::Sound mushCollected;

	sf::SoundBuffer powerUpAppearsBuffer;
	sf::Sound powerUpAppearsSound;
	sf::SoundBuffer gumbaDeadBuffer;
	sf::Sound gumbaDeadSound;
	sf::SoundBuffer enterPipeBuffer;
	sf::Sound enterPipeSound;
	sf::SoundBuffer oneUPBuffer;
	sf::Sound oneUPSound;
	sf::SoundBuffer breakBlockBuffer;
	sf::Sound breakBlockSound;

	sf::Texture EnemySpritesheet;
	sf::Texture* Tileset;

	bool oneTime = true;
	void resetLogic();

	void createStaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect);
	void destroyStaticTile(sf::Vector2f pos);

	void createCoinTile(sf::Texture& tileset, sf::Vector2f pos, int type);
	void destroyCoinTile(sf::Vector2f pos);

	void convertToBackground(sf::Sprite s);
	void destroyBackgroundTile(sf::Vector2f pos);

	void createTransportTile(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f ToPos, sf::Vector2f CameraPos, sf::Vector2f dir);
	void destroyTransportTile(sf::Vector2f pos);

	void createPickupTile(sf::Vector2f pos, int type, sf::Texture& tileset);
	void destroyPickupTile(sf::Vector2f pos);

	void createEnemy(sf::Vector2f position, int t);
	void destroyEnemy(sf::Sprite& Preview);

	void createParticle(sf::Vector2f position, sf::Vector2f vel, sf::Texture* TileSet, sf::IntRect TextRect);

	void createPoleTile(sf::Texture& tileSet, sf::Vector2f pos, int side);

	void createCastleTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect);
	void destroyCastleTile(sf::Vector2f pos);

	void createDeathTrigger(sf::Vector2f pos);
	void destroyDeathTrigger(sf::Vector2f pos);

	void createFireEntity(sf::Vector2f pos, int dir);

	void Logic(Player& Mario);
	void DrawToScreen(sf::RenderWindow& window, Player& player, bool Debug);
	void DrawToScreen(sf::RenderWindow& window, Player& player, Player& player2, bool Debug);
	void saveToFile(string FileName);
	void LoadFromFile(string FileName, sf::Texture& tileset, sf::Color& backgroundColor, Player& Mario, Player& Luigi, Entity& EntityList);
};