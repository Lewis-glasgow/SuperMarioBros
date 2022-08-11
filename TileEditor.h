#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.h"

class TileEditor
{
private:
	string CurrentMode = "";
	bool canPlace = true;
	int typeTile = 0;
public:
	string MapName = "";
	string Logic(string Mapname);
	void showIcon(sf::RenderWindow& window, sf::Sprite& icon, string& CurrentMode, sf::View& v, sf::Vector2f mousePos, sf::IntRect& TileRect, sf::Texture& TileSet, sf::Texture& EnemySpriteSheet, sf::Texture& MarioSpriteSheet);
};

