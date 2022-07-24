#pragma once
#include "TileEditor.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include <fstream>
using namespace std;

class coinTile {
public:
	sf::Sprite s;
	sf::Clock FrameCounter;
	int xFrame = 759;
	bool activated = false;
	int type = 0;
	coinTile(sf::Texture& tileset, sf::Vector2f pos, int t);
	bool Logic(Player& Mario, sf::Sound& sound);
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
	void Logic(Player& Mario, bool oneTimes);
};

class pickupTile {
public:
	sf::Sprite sprite;
	bool activated = false;
	int type = 0;
	float direction = 1;
	int speed = 1;
	sf::Clock waitTime;
	pickupTile(sf::Vector2f pos, int t, sf::Texture& tileset);
	void collision(vector<sf::Sprite>& tiles, sf::Vector2f vel);
	bool Logic(Player& Mario, vector<sf::Sprite>& tiles, bool oneTime);
};

class Entity
{
public:
	vector<sf::Sprite> StaticTiles;
	vector<coinTile> CoinTiles;
	vector<sf::Sprite> AllTiles;
	vector<sf::Sprite> backgroundTiles;
	vector<pipeTransportTile> transportTiles;
	vector<pickupTile> pickupTiles;

	sf::SoundBuffer coinCollectedBuffer;
	sf::Sound coinCollected;
	sf::SoundBuffer mushCollectedBuffer;
	sf::Sound mushCollected;

	bool oneTime = true;
	void resetLogic();

	void createStaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect);
	void destroyStaticTile(sf::Vector2f pos);
	void createCoinTile(sf::Texture& tileset, sf::Vector2f pos, int type);
	void destroyCoinTile(sf::Vector2f pos);
	void convertToBackground(sf::Vector2f pos);
	void createTransportTile(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f ToPos, sf::Vector2f CameraPos, sf::Vector2f dir);
	void destroyTransportTile(sf::Vector2f pos);

	void createPickupTile(sf::Vector2f pos, int type, sf::Texture& tileset);
	void destroyPickupTile(sf::Vector2f pos);

	void Logic(Player& Mario);
	void DrawToScreen(sf::RenderWindow& window, Player& player, bool Debug);
	void DrawToScreen(sf::RenderWindow& window, Player& player, Player& player2, bool Debug);
	void saveToFile(string FileName);
	void LoadFromFile(string FileName, sf::Texture& tileset);
};