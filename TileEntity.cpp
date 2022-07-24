#include "TileEntity.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

coinTile::coinTile(sf::Texture& tileset, sf::Vector2f pos, int t) {
	s.setTexture(tileset);
	s.setPosition(pos);
	type = t;
	if (type == 0) {
		s.setTextureRect(sf::IntRect(759, 0, 32, 32));
	}
	else {
		s.setTextureRect(sf::IntRect(759, 33, 32, 32));
	}
	s.setScale(sf::Vector2f(0.5f, 0.5f));

}
bool coinTile::Logic(Player& Mario, sf::Sound& sound) {
	if (FrameCounter.getElapsedTime().asSeconds() > 0.2 && !activated) {
		xFrame += 33;
		if (xFrame > 825 && !activated) {
			xFrame = 759;
		}
		if (type == 0) {
			s.setTextureRect(sf::IntRect(xFrame, 0, 32, 32));
		}
		else if (type == 1) {
			s.setTextureRect(sf::IntRect(xFrame, 33, 32, 32));
		}
		FrameCounter.restart();
	}
	sf::FloatRect MRect(Mario.hitbox.getGlobalBounds());
	if (type == 0) {
		MRect.top -= 2;
		if (s.getGlobalBounds().intersects(MRect)) {
			if (s.getGlobalBounds().left < MRect.left + MRect.width && MRect.left < s.getGlobalBounds().left+s.getGlobalBounds().width) {
				if (Mario.vel.y < 0) {
					xFrame = 858;
					activated = true;
					s.setTextureRect(sf::IntRect(xFrame, 0, 32, 32));
					Mario.score += 1; 
					sound.play();
					return false;
				}
			}
		}
	}
	else if (type == 1) {
		if (s.getGlobalBounds().intersects(MRect)) {
			Mario.score += 1;
			sound.play();
			return true;
		}
	}
};

pickupTile::pickupTile(sf::Vector2f pos, int t, sf::Texture& tileset) {
	sprite.setTexture(tileset);
	sprite.setScale(0.5, 0.5);
	sprite.setPosition(pos);
	type = t;

	if (type == 0) {
		sprite.setTextureRect(sf::IntRect(825, 264, 32, 32));
	}

}

void pickupTile::collision(vector<sf::Sprite>& tiles, sf::Vector2f vel) {
	for (int tile = 0; tile < tiles.size(); tile++) {
		if (sprite.getGlobalBounds().intersects(tiles[tile].getGlobalBounds())) {
			if (vel.x > 0) {
				sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left - sprite.getGlobalBounds().width, sprite.getPosition().y));
				direction = -1;
			}
			if (vel.x < 0) {
				sprite.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left + tiles[tile].getGlobalBounds().width, sprite.getPosition().y));
				direction = 1;
			}
			if (vel.y > 0) {
				sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top - sprite.getGlobalBounds().height));
			}
			if (vel.y < 0) {
				sprite.setPosition(sf::Vector2f(sprite.getPosition().x, tiles[tile].getGlobalBounds().top + tiles[tile].getGlobalBounds().height));
			}
		}
	}
}

void Entity::destroyPickupTile(sf::Vector2f pos) {
	for (int i = 0; pickupTiles.size(); i++) {
		if (pickupTiles[i].sprite.getPosition() == pos) {
			pickupTiles.erase(pickupTiles.begin() + i);
		}
	}
}
bool pickupTile::Logic(Player& Mario, vector<sf::Sprite>& tiles, bool oneTime) {

	if (type == 0) {
		if (!activated) {
			sf::FloatRect MRect = Mario.sprite.getGlobalBounds();
			MRect.top -= 2;
			if (sprite.getGlobalBounds().intersects(MRect)) {
				activated = true;
				waitTime.restart();
			}
		}
		else {
			if (waitTime.getElapsedTime().asSeconds() < 0.3) {
				if (oneTime) {
					sprite.move(sf::Vector2f(0, -1));
				}
			}
			else {
				sf::Vector2f vel = sf::Vector2f(direction * speed, 0);
				if (oneTime) {
					sprite.move(vel);
				}
				collision(tiles, vel);

				vel = sf::Vector2f(0, 3);
				if (oneTime) {
					sprite.move(vel);
				}
				collision(tiles, vel);

				if (sprite.getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds())) {
					Mario.smallMario = false;
					return true;
				}
			}
		}
	}
	return false;
}

void Entity::saveToFile(string FileName) {
	ofstream File;
	File.open(FileName);
	if (File.is_open()) {
		for (int i = 0; i < StaticTiles.size(); i++) {
			File << "StaticTiles," <<
				StaticTiles[i].getPosition().x 
				<< "," << StaticTiles[i].getPosition().y 
				<< "," << StaticTiles[i].getTextureRect().left
				<< "," << StaticTiles[i].getTextureRect().top 
				<< "," << StaticTiles[i].getTextureRect().width 
				<< "," << StaticTiles[i].getTextureRect().height << "\n";
		}

		for (int i = 0; i < CoinTiles.size(); i++) {
			File << "CoinTiles," <<
				CoinTiles[i].s.getPosition().x 
				<< "," << CoinTiles[i].s.getPosition().y 
				<< "," << CoinTiles[i].type << "\n";
		}

		for (int i = 0; i < backgroundTiles.size(); i++) {
			File << "backgroundTiles," <<
				backgroundTiles[i].getPosition().x 
				<< "," << backgroundTiles[i].getPosition().y 
				<< "," << backgroundTiles[i].getTextureRect().left 
				<< "," << backgroundTiles[i].getTextureRect().top 
				<< "," << backgroundTiles[i].getTextureRect().width 
				<< "," << backgroundTiles[i].getTextureRect().height << "\n";
		}
		for (int i = 0; i < transportTiles.size(); i++) {
			File << "transportTiles," <<
				transportTiles[i].s.getPosition().x 
				<< "," << transportTiles[i].s.getPosition().y 
				<< "," << transportTiles[i].s.getSize().x 
				<< "," << transportTiles[i].s.getSize().y 
				<< "," << transportTiles[i].toPos.x 
				<< "," << transportTiles[i].toPos.y 
				<< "," << transportTiles[i].cameraPos.x 
				<< "," << transportTiles[i].cameraPos.y 
				<< "," << transportTiles[i].dir.x 
				<< "," << transportTiles[i].dir.y << "\n";
		}
		for (int i = 0; i < pickupTiles.size(); i++) {
			File << "pickupTiles," <<
				pickupTiles[i].sprite.getPosition().x
				<< "," << pickupTiles[i].sprite.getPosition().y
				<< "," << pickupTiles[i].type << "\n";
		}

		File.close();
	}
}

void Entity::LoadFromFile(string FileName, sf::Texture& tileset) {
	coinCollectedBuffer.loadFromFile("assets/audio/coin.wav");
	coinCollected.setBuffer(coinCollectedBuffer);

	ifstream File(FileName);
	string line;
	if (File.is_open()) {
		while (getline(File, line)) {

			if (line.substr(0, 11) == "StaticTiles") {
				vector<string> properties = { "0","0","0","0","0","0" };
				int index = 0;
				for (int i = 12; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				sf::IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
				createStaticTile(tileset, pos, rect);
			}
			else if (line.substr(0, 9) == "CoinTiles") {
				vector<string> properties = { "0","0", "0"};
				int index = 0;
				for (int i = 10; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				int type = stoi(properties[2]);
				createCoinTile(tileset, pos, type);
			}
			else if (line.substr(0, 15) == "backgroundTiles") {
				vector<string> properties = { "0","0","0","0","0","0" };
				int index = 0;
				for (int i = 16; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				sf::IntRect rect(stoi(properties[2]), stoi(properties[3]), stoi(properties[4]), stoi(properties[5]));
				sf::Sprite s;
				s.setScale(sf::Vector2f(0.5, 0.5));
				s.setPosition(pos);
				s.setTexture(tileset);
				s.setTextureRect(rect);
				backgroundTiles.push_back(s);
			}
			else if (line.substr(0, 14) == "transportTiles") {
				vector<string> properties = { "0","0","0","0","0","0", "0", "0", "0", "0"};
				int index = 0;
				for (int i = 15; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				sf::Vector2f size(stoi(properties[2]), stoi(properties[3]));
				sf::Vector2f toPos(stoi(properties[4]), stoi(properties[5]));
				sf::Vector2f camPos(stoi(properties[6]), stoi(properties[7]));
				sf::Vector2f dir(stoi(properties[8]), stoi(properties[9]));
				createTransportTile(pos, size, toPos, camPos, dir);
			}
			else if (line.substr(0, 11) == "pickupTiles") {
				vector<string> properties = { "0","0", "0" };
				int index = 0;
				for (int i = 12; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				int type = stoi(properties[2]);
				createPickupTile(pos, type, tileset);
			}

		}
	}
}

void Entity::createStaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect){
	bool canPlace = true;
	for (int i = 0; i < StaticTiles.size(); i++) {
		if (StaticTiles[i].getPosition() == pos) {
			canPlace = false;
		}
	}
	if (canPlace) {
		sf::Sprite s;
		s.setTexture(tileset);
		s.setPosition(pos);
		s.setTextureRect(rect);
		s.setScale(sf::Vector2f(0.5, 0.5));
		StaticTiles.push_back(s);
		AllTiles.push_back(s);
	}
}

void Entity::destroyStaticTile(sf::Vector2f pos) {
	for (int i = 0; i < StaticTiles.size(); i++) {
		if (StaticTiles[i].getPosition() == pos) {
			StaticTiles.erase(StaticTiles.begin() + i);
		}
	}
	for (int i = 0; i < backgroundTiles.size(); i++) {
		if (backgroundTiles[i].getPosition() == pos) {
			backgroundTiles.erase(backgroundTiles.begin() + i);
		}
	}
	for (int i = 0; i < AllTiles.size(); i++) {
		if (AllTiles[i].getPosition() == pos) {
			AllTiles.erase(AllTiles.begin() + i);
		}
	}
}
void Entity::DrawToScreen(sf::RenderWindow& window, Player& player, bool Debug) {

	for (int i = 0; i < backgroundTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(backgroundTiles[i].getGlobalBounds()) || Debug) {
			window.draw(backgroundTiles[i]);
		}
	}
	for (int i = 0; i < pickupTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(pickupTiles[i].sprite.getGlobalBounds()) || Debug) {
			window.draw(pickupTiles[i].sprite);
		}
	}

	window.draw(player.sprite);

	for (int i = 0; i < StaticTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(StaticTiles[i].getGlobalBounds()) || Debug) {
			window.draw(StaticTiles[i]);
		}
	}
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(CoinTiles[i].s);
		}
	}
	if (Debug) {
		for (int i = 0; i < transportTiles.size(); i++) {
			if (player.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || Debug) {
				window.draw(transportTiles[i].s);
			}
		}
	}
	//window.draw(player.areaBounds);
	//window.draw(Mario.hitbox);
}

void Entity::DrawToScreen(sf::RenderWindow& window, Player& player, Player& player2, bool Debug) {
	for (int i = 0; i < backgroundTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(backgroundTiles[i].getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(backgroundTiles[i].getGlobalBounds()) || Debug) {
			window.draw(backgroundTiles[i]);
		}
	}
	for (int i = 0; i < pickupTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(pickupTiles[i].sprite.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(pickupTiles[i].sprite.getGlobalBounds()) || Debug) {
			window.draw(pickupTiles[i].sprite);
		}
	}

	window.draw(player.sprite);
	window.draw(player2.sprite);

	for (int i = 0; i < StaticTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(StaticTiles[i].getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(StaticTiles[i].getGlobalBounds()) || Debug) {
			window.draw(StaticTiles[i]);
		}
	}
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(CoinTiles[i].s);
		}
	}
	if (Debug) {
		for (int i = 0; i < transportTiles.size(); i++) {
			if (player.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || Debug) {
				window.draw(transportTiles[i].s);
			}
		}
	}
	//window.draw(Mario.hitbox);
}

void Entity::createCoinTile(sf::Texture& tileset, sf::Vector2f pos, int type) {
	bool canPlace = true;
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (CoinTiles[i].s.getPosition() == pos) {
			canPlace = false;
		}
	}
	if (canPlace) {
		coinTile tile(tileset, pos, type);
		CoinTiles.push_back(tile);
		if (tile.type == 0) {
			AllTiles.push_back(tile.s);
		}
	}
}
void Entity::destroyCoinTile(sf::Vector2f pos) {
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (CoinTiles[i].s.getPosition() == pos) {
			CoinTiles.erase(CoinTiles.begin() + i);
		}
	}
	for (int i = 0; i < AllTiles.size(); i++) {
		if (AllTiles[i].getPosition() == pos) {
			AllTiles.erase(AllTiles.begin() + i);
		}
	}
}

void Entity::Logic(Player& Mario) {
	for (int i = 0; i < CoinTiles.size(); i++) {
		bool destroyCoin = CoinTiles[i].Logic(Mario, coinCollected);
		if (destroyCoin && CoinTiles[i].type == 1) {
			destroyCoinTile(CoinTiles[i].s.getPosition());
		}
	}
	for (int i = 0; i < transportTiles.size(); i++) {
		transportTiles[i].Logic(Mario, oneTime);
	}
	for (int i = 0; i < pickupTiles.size(); i++) {
		bool destroyPickup = pickupTiles[i].Logic(Mario, AllTiles, oneTime);
		if (destroyPickup) {
			destroyPickupTile(pickupTiles[i].sprite.getPosition());
		}
	}
	for (int i = 0; i < StaticTiles.size(); i++) {
		sf::FloatRect MRect = Mario.hitbox.getGlobalBounds();
		MRect.top -= 2;
		if (MRect.intersects(StaticTiles[i].getGlobalBounds()) && Mario.vel.y < 0) {
			if (!Mario.smallMario) {
				for (int o = 0; o < AllTiles.size(); o++) {
					if (AllTiles[o].getPosition() == StaticTiles[i].getPosition()) {
						AllTiles.erase(AllTiles.begin() + o);
					}
				}
				StaticTiles.erase(StaticTiles.begin() + i);
			}
		}
	}
	oneTime = false;
}

void Entity::convertToBackground(sf::Vector2f pos) {
	for (int i = 0; i < StaticTiles.size(); i++) {
		if (StaticTiles[i].getPosition() == pos) {
			backgroundTiles.push_back(StaticTiles[i]);
			StaticTiles.erase(StaticTiles.begin() + i);
		}
	}
}

pipeTransportTile::pipeTransportTile(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f ToPos, sf::Vector2f CameraPos, sf::Vector2f dir) {
	s.setSize(size);
	s.setPosition(pos);
	toPos = ToPos;
	cameraPos = CameraPos;
	this->dir = dir;
}

void Entity::createTransportTile(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f ToPos, sf::Vector2f CameraPos, sf::Vector2f dir) {
	bool canPlace = true;
	for (int i = 0; i < transportTiles.size(); i++) {
		if (pos + sf::Vector2f(8, 0) == transportTiles[i].s.getPosition()) {
			canPlace = false;
		}
	}
	if (canPlace) {
		pipeTransportTile tile(pos, size, ToPos, CameraPos, dir);
		transportTiles.push_back(tile);
	}
}
void Entity::destroyTransportTile(sf::Vector2f pos) {
	for (int i = 0; i < transportTiles.size(); i++) {
		if (pos + sf::Vector2f(8, 0) == transportTiles[i].s.getPosition()) {
			transportTiles.erase(transportTiles.begin() + i);
		}
	}
}
void pipeTransportTile::Logic(Player& Mario, bool oneTime) {
	if (s.getGlobalBounds().intersects(Mario.sprite.getGlobalBounds())) {
		if (s.getGlobalBounds().left <= Mario.hitbox.getGlobalBounds().left && s.getGlobalBounds().left + s.getGlobalBounds().width >= Mario.hitbox.getGlobalBounds().left + Mario.hitbox.getGlobalBounds().width) {
			if (!active && !Mario.MoveLockState) {
				if (((sf::Keyboard::isKeyPressed(sf::Keyboard::S) && Mario.playerNum == 0)|| sf::Joystick::getAxisPosition(Mario.playerNum, sf::Joystick::Y) > Mario.axisThreshold) && dir.y == 1 ||
					((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && Mario.playerNum == 0) || (sf::Joystick::getAxisPosition(Mario.playerNum, sf::Joystick::X) > Mario.axisThreshold)) && dir.x == 1) {
					active = true;
					cout << "yes";
					Mario.MoveLockState = true;
					Mario.camLockState = !Mario.camLockState;
				}
			}
		}
	}

	if (active && oneTime) {
		if (oneTime) {
			Mario.sprite.move(dir);
		}
		if (dir.x != 0) {
			Mario.Animate("run");
			Mario.sprite.setScale(sf::Vector2f(-1, 1));
			if (Mario.smallMario) {
				Mario.sprite.setPosition(Mario.sprite.getPosition().x, s.getPosition().y + 8);
			}
			else {
				Mario.sprite.setPosition(Mario.sprite.getPosition().x, s.getPosition().y + 2);
			}
		}
		else {
			Mario.Animate("idle");
		}
		timer += 1;

		if (timer >= 75) {
			Mario.Camera.setCenter(cameraPos);
			Mario.sprite.setPosition(toPos);
			active = false;
			Mario.MoveLockState = false;
		}
	}
}

void Entity::createPickupTile(sf::Vector2f pos, int type, sf::Texture& tileset) {
	bool canPlace = true;
	for (int i = 0; i < pickupTiles.size(); i++) {
		if (pickupTiles[i].sprite.getPosition() == pos) {
			canPlace = false;
		}
	}

	if (canPlace) {
		pickupTile Tile(pos, type, tileset);
		pickupTiles.push_back(Tile);
	}
}

void Entity::resetLogic() {
	oneTime = true;
}