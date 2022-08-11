#include "TileEntity.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

StaticTile::StaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect) {
	s.setTexture(tileset);
	s.setPosition(pos);
	startPos = pos;
	s.setTextureRect(rect);
	s.setScale(sf::Vector2f(0.5, 0.5));
}

int StaticTile::logic(Player& player, sf::FloatRect PRect, vector<Enemy>& Enemies) {
	if (s.getGlobalBounds().intersects(PRect) && player.vel.y < 0) {
		if (!player.smallMario) {
			sf::FloatRect Rect;
			Rect = s.getGlobalBounds();
			Rect.top -= 16;
			for (int i = 0; i < Enemies.size(); i++) {
				if (Enemies[i].s.getGlobalBounds().intersects(Rect)) {
					Enemies[i].activated = 2;
					Enemies[i].grav = -10;
				}
			}
			return 1;
		}
		else {
			if (!moved) {
				s.move(0, -2);
				TimeCount.restart();
				moved = true;
			}
		}
	}
	if (moved) {
		if (TimeCount.getElapsedTime().asSeconds() > 0.1) {
			s.move(0, 2);
			moved = false;
		}
	}
	return 0;
}

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
bool coinTile::Logic(Player& Mario, sf::Sound& sound, vector<Enemy>& Enemies) {
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
					if (!activated) {
						xFrame = 858;
						activated = true;
						s.setTextureRect(sf::IntRect(xFrame, 0, 32, 32));
						Mario.score += 1; 

						sf::FloatRect Rect;
						Rect = s.getGlobalBounds();
						Rect.top -= 16;
						for (int i = 0; i < Enemies.size(); i++) {
							if (Enemies[i].s.getGlobalBounds().intersects(Rect)){
								Enemies[i].activated = 2;
								Enemies[i].grav = -5;
							}
						}
						return false;
					}
				}
			}
		}
	}
	else if (type == 1) {
		if (s.getGlobalBounds().intersects(MRect)) {
			Mario.coins += 1;
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
	else if (type == 1) {
		sprite.setTextureRect(sf::IntRect(825, 330, 32, 32));
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
	for (int o = 0; o < pickupTiles.size(); o++) {
		if (pos == pickupTiles[o].sprite.getPosition()) {
			pickupTiles.erase(pickupTiles.begin() + o);
			break;
		}
	}
}
bool pickupTile::Logic(Player& Mario, vector<sf::Sprite>& tiles, bool oneTime, sf::Sound& mushSound, sf::Sound& activatedSound) {

	if (!activated) {
		sf::FloatRect MRect = Mario.hitbox.getGlobalBounds();
		MRect.top -= 2;
		if (sprite.getGlobalBounds().intersects(MRect) && Mario.vel.y < 0) {
			if (sprite.getGlobalBounds().left < MRect.left + MRect.width && MRect.left < sprite.getGlobalBounds().left + sprite.getGlobalBounds().width) {
				activated = true;
				activatedSound.play();
				waitTime.restart();
				startPos = sprite.getPosition();
				if (!Mario.smallMario && (type == 0 || type == 1)) {
					type = 1;
				}
				else {
					type = 0;
				}
			}
		}
	}
	if (type == 0) {
		sprite.setTextureRect(sf::IntRect(825, 264, 32, 32));
		if (activated) {
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
					if (Mario.smallMario) {
						Mario.MoveLockState = true;
						Mario.Animate("mushroom");
						mushSound.play();
						return true;
					}
				}
			}
		}
	}
	else if (type == 1) {
		sprite.setTextureRect(sf::IntRect(825, 330, 32, 32));
		if (activated) {
			if (sprite.getPosition().y + sprite.getGlobalBounds().height > startPos.y) {
				if (oneTime) {
					sprite.move(sf::Vector2f(0, -1));
				}
			}
			else {
				if (sprite.getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds())) {
					Mario.fire = true;
					Mario.smallMario = false;
					return true;
				}
			}
		}
	}
	return false;
}

void Entity::destroyEnemy(sf::Sprite& Preview) {
	for (int i = 0; i < Enemies.size(); i++) {
		if (Preview.getGlobalBounds().intersects(Enemies[i].s.getGlobalBounds())) {
			Enemies.erase(Enemies.begin() + i);
		}
	}
}

Enemy::Enemy(sf::Texture& spritesheet, sf::Vector2f position, int t, bool d) {
	type = t;
	debug = d;
	s.setTexture(spritesheet);
	s.setPosition(position);
	if (type == 0) {
		s.setTextureRect(sf::IntRect(0, 4, 16, 16));
		s.setOrigin(8, 8);
		collisionOffset = sf::Vector2f(8, 8);
	}
	if (type == 1) {
		s.setTextureRect(sf::IntRect(180, 0, 16, 23));
		s.setOrigin(8, 11.5);
		collisionOffset = sf::Vector2f(8, 11.5);
	}
}

bool Enemy::Logic(Player& Mario, bool oneTime, vector<sf::Sprite>& Tiles, vector<Enemy>& Enemies, sf::Sound& gumbaDead) {
	if (type == 0) {
		if (s.getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds())) {
			if (Mario.vel.y > 0.3f && !Mario.isOnFloor) {
				if (activated == 0) {
					activated = 1;
					gumbaDead.play();
					FrameCounter.restart();
					s.move(0, 4);
					Mario.grav = -6.2;
					Mario.score += 100;
				}
			}
			else {
				if (activated == 0 && Mario.CanBeHit) {
					s.move(-direction, 0);
					moved = true;
					if (Mario.finished == 0) {
						if ((!Mario.smallMario || Mario.fire)) {
							Mario.MoveLockState = true;
							Mario.fire = false;
							Mario.Animate("shrink");
						}
						else {
							Mario.dead = true;
							//Mario.grav = -12;
						}
					}
					direction = -direction;
				}
			}
		}
		yframe = 4;
		if (activated == 0) {
			if (FrameCounter.getElapsedTime().asSeconds() > 0.25) {
				if (xframe == 0) {
					xframe += 30;
				}
				else {
					xframe = 0;
				}
				FrameCounter.restart();
			}
		}
		else {
			if (activated == 1) {
				xframe = 60;
			}
		}
		s.setTextureRect(sf::IntRect(xframe, yframe, 16, 16));
		if (!debug && activated == 0) {
			if (!moved) {
				s.move(direction * speed, 0);
				collision(Tiles, sf::Vector2f(direction * speed, 0), Mario, Enemies);
				s.move(0, 3);
				collision(Tiles, sf::Vector2f(0, 1), Mario, Enemies);
				moved = true;
			}
		}
		if (activated == 1) {
			if (FrameCounter.getElapsedTime().asSeconds() > 1) {
				return true;
			}
		}
		else if (activated == 2 || activated == -1) {
			s.setRotation(180);
			if (!moved) {
				s.move(direction, grav);
				grav += 0.3;
				moved = true;
			}
		}

		return false;
	}

	else if (type == 1) {
		yframe = 0;
		if (direction != 0) {
			s.setScale(-direction, 1);
		}
		if (activated == 0) {
			if (FrameCounter.getElapsedTime().asSeconds() > 0.2) {
				if (xframe == 150) {
					xframe = 180;
				}
				else {
					xframe = 150;
				}
				FrameCounter.restart();
			}

			if (!debug && !moved) {
				s.move(direction * speed, 0);
				collision(Tiles, sf::Vector2f(direction * speed, 0), Mario, Enemies);
				s.move(0, 3);
				collision(Tiles, sf::Vector2f(0, 1), Mario, Enemies);
				moved = false;
			}
		}
		if (s.getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds()) && activated == 0) {
			if (Mario.vel.y > 0 && !Mario.isOnFloor) {
				activated = 1;
				direction = 0;
				Mario.grav = -6.2;
				xframe = 360;
				s.setOrigin(8, 7);
				collisionOffset = sf::Vector2f(8, 7);
				FrameCounter.restart();
				return false;
			}
			else {
				cout << "ye\n";
				if (Mario.CanBeHit && Mario.finished == 0) {
					direction = -direction;
					if (Mario.smallMario && Mario.CanBeHit) {
						Mario.dead = true;
						//Mario.MoveLockState = true;
					}
					else {
						if (Mario.CanBeHit) {
							Mario.MoveLockState = true;
							Mario.fire = false;
							Mario.Animate("shrink");
						}
					}
				}
			}
		}
		if (FrameCounter.getElapsedTime().asSeconds() > 5 && activated == 1) {
			activated = 0;
			direction = -1;
			s.move(0, -4.5);
			s.setOrigin(8, 11.5);
			collisionOffset = sf::Vector2f(8, 11.5);
		}
		if (s.getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds()) && activated == 1) {
			if (Mario.vel.y > 0) {
				if (Mario.sprite.getPosition().x < s.getPosition().x) {
					direction = 1;
				}
				else {
					direction = -1;
				}
				Mario.grav = -6.2;
				activated = 2;
			}
		}
		if (activated == 2 || activated == 1) {
			if (!debug && !moved) {
				s.move((direction * 5)* speed, 0);
				collision(Tiles, sf::Vector2f((direction*5) * speed, 0), Mario, Enemies);
				
				s.move(0, 3);
				collision(Tiles, sf::Vector2f(0, 1), Mario, Enemies);
				moved = true;
			}
		}
		else if (activated == -1) {
			if (!moved) {
				xframe = 360;
				s.setOrigin(8, 7);
				collisionOffset = sf::Vector2f(8, 7);
				s.setRotation(180);
				s.move(direction, grav);
				grav += 0.3;
				moved = true;
			}
		}
	}

	if ((activated == 2 || activated == 1) && type == 1) {
		s.setTextureRect(sf::IntRect(360, 5, 16, 14));
	}
	else {
		s.setTextureRect(sf::IntRect(xframe, yframe, 16, 23));
	}
}

void Enemy::collision(vector<sf::Sprite>& tiles, sf::Vector2f vel, Player& Mario, vector<Enemy>& Enemies) {
	for (int tile = 0; tile < tiles.size(); tile++) {
		if (s.getGlobalBounds().intersects(tiles[tile].getGlobalBounds())) {
			if (vel.x > 0) {
				s.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left - collisionOffset.x, s.getPosition().y));
				direction = -1;
			}
			if (vel.x < 0) {
				s.setPosition(sf::Vector2f(tiles[tile].getGlobalBounds().left + collisionOffset.x + tiles[tile].getGlobalBounds().width, s.getPosition().y));
				direction = 1;
			}
			if (vel.y > 0) {
				s.setPosition(sf::Vector2f(s.getPosition().x, tiles[tile].getGlobalBounds().top - collisionOffset.y));
			}
			if (vel.y < 0) {
				s.setPosition(sf::Vector2f(s.getPosition().x, tiles[tile].getGlobalBounds().top + collisionOffset.y + tiles[tile].getGlobalBounds().height));
			}
		}
	}
	for (int tile = 0; tile < Enemies.size(); tile++) {
		if (s.getGlobalBounds().intersects(Enemies[tile].s.getGlobalBounds()) && Enemies[tile].s.getPosition() != s.getPosition()) {
			if (activated == 2 && type == 1) {
				if (Enemies[tile].activated == 0) {
					Enemies[tile].activated = 2;
					Enemies[tile].grav = -10;
				}
			}
			else {
				if (vel.x > 0) {
					direction = -1;
				}
				if (vel.x < 0) {
					direction = 1;
				}
			}
		}
	}

	if (activated == 2 && type == 1) {
		if (s.getGlobalBounds().intersects(Mario.sprite.getGlobalBounds())) {
			if (Mario.isOnFloor) {
				if (!Mario.smallMario && Mario.CanBeHit) {
					Mario.MoveLockState = true;
					Mario.Animate("shrink");
				}
				else {
					if (Mario.CanBeHit) {
						Mario.dead = true;
					}
				}
				direction = -direction;
			}
		}
	}
}

void Entity::saveToFile(string FileName) {
	ofstream File;
	File.open(FileName+".wd");
	
	if (File.is_open()) {
		if (BGColor != sf::Color(93, 148, 251)) {
			File << "backgroundColor," << BGColor.r << "," << BGColor.g << "," << BGColor.b << "\n";
		}
		File << "CameraPosition," << CamPos.x << "," << CamPos.y << "\n";
		for (int i = 0; i < StaticTiles.size(); i++) {
			File << "StaticTiles," <<
				StaticTiles[i].s.getPosition().x 
				<< "," << StaticTiles[i].s.getPosition().y 
				<< "," << StaticTiles[i].s.getTextureRect().left
				<< "," << StaticTiles[i].s.getTextureRect().top 
				<< "," << StaticTiles[i].s.getTextureRect().width 
				<< "," << StaticTiles[i].s.getTextureRect().height << "\n";
		}
		for (int i = 0; i < castleTiles.size(); i++) {
			File << "castleTiles," <<
				castleTiles[i].getPosition().x
				<< "," << castleTiles[i].getPosition().y
				<< "," << castleTiles[i].getTextureRect().left
				<< "," << castleTiles[i].getTextureRect().top
				<< "," << castleTiles[i].getTextureRect().width
				<< "," << castleTiles[i].getTextureRect().height << "\n";
		}
		for (int i = 0; i < deathTriggers.size(); i++) {
			File << "deathTrigger," <<
				deathTriggers[i].getPosition().x
				<< "," << deathTriggers[i].getPosition().y
				<< "," << deathTriggers[i].getSize().x
				<< "," << deathTriggers[i].getSize().y << "\n";
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
		for (int i = 0; i < Enemies.size(); i++) {
			File << "Enemies," <<
				Enemies[i].s.getPosition().x
				<< "," << Enemies[i].s.getPosition().y
				<< "," << Enemies[i].type << "\n";
		}
		for (int i = 0; i < Poles.size(); i++) {
			File << "Poles," <<
				Poles[i].s.getPosition().x
				<< "," << Poles[i].s.getPosition().y
				<< "," << Poles[i].side << "\n";
		}

		File.close();
	}
}

void Entity::LoadFromFile(string FileName, sf::Texture& tileset, sf::Color& backgroundColor, Player& Mario, Player& Luigi, Entity& EntityList) {
	coinCollectedBuffer.loadFromFile("assets/audio/coin.wav");
	coinCollected.setBuffer(coinCollectedBuffer);
	mushCollectedBuffer.loadFromFile("assets/audio/mushroom.wav");
	mushCollected.setBuffer(mushCollectedBuffer);

	powerUpAppearsBuffer.loadFromFile("assets/audio/powerUpAppear.wav");
	powerUpAppearsSound.setBuffer(powerUpAppearsBuffer);
	gumbaDeadBuffer.loadFromFile("assets/audio/goomba.wav");
	gumbaDeadSound.setBuffer(gumbaDeadBuffer);
	enterPipeBuffer.loadFromFile("assets/audio/pipe.wav");
	enterPipeSound.setBuffer(enterPipeBuffer);
	oneUPBuffer.loadFromFile("assets/audio/");
	oneUPSound.setBuffer(oneUPBuffer);
	breakBlockBuffer.loadFromFile("assets/audio/block.wav");
	breakBlockSound.setBuffer(breakBlockBuffer);

	EnemySpritesheet.loadFromFile("assets/enemies_sprites.png");
	Tileset = &tileset;

	BGColor = sf::Color(93, 148, 251);

	EntityList.StaticTiles.clear();
	EntityList.castleTiles.clear();
	EntityList.CoinTiles.clear();
	EntityList.AllTiles.clear();
	EntityList.backgroundTiles.clear();
	EntityList.transportTiles.clear();
	EntityList.pickupTiles.clear();
	EntityList.Enemies.clear();
	EntityList.deathTriggers.clear();
	EntityList.particles.clear();
	EntityList.FireBalls.clear();
	EntityList.Poles.clear();

	ifstream File(FileName);
	string line;
	if (File.is_open()) {
		while (getline(File, line)) {
			if (line.substr(0, 15) == "backgroundColor") {
				vector<string> properties = { "0","0","0" };
				int index = 0;
				for (int i = 16; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Color c(stoi(properties[0]), stoi(properties[1]), stoi(properties[2]));
				backgroundColor = c;
				BGColor = c;
			}
			if (line.substr(0, 14) == "CameraPosition") {
				vector<string> properties = { "0","0" };
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
				Mario.Camera.setCenter(pos);
				Luigi.Camera.setCenter(pos);
				CamPos = pos;
			}
			else if (line.substr(0, 11) == "StaticTiles") {
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
			else if (line.substr(0, 11) == "castleTiles") {
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
				createCastleTile(tileset, pos, rect);
			}
			else if (line.substr(0, 12) == "deathTrigger") {
				vector<string> properties = { "0","0","0","0"};
				int index = 0;
				for (int i = 13; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				sf::Vector2f size(stoi(properties[2]), stoi(properties[3]));
				createDeathTrigger(pos);
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
			else if (line.substr(0, 7) == "Enemies") {
				vector<string> properties = { "0","0", "0" };
				int index = 0;
				for (int i = 8; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				int type = stoi(properties[2]);
				createEnemy(pos, type);
			}
			else if (line.substr(0, 5) == "Poles") {
				vector<string> properties = { "0","0", "0" };
				int index = 0;
				for (int i = 6; i < line.length(); i++) {
					if (line.at(i) == ',') {
						index += 1;
					}
					else {
						properties[index] += line.at(i);
					}
				}
				sf::Vector2f pos(stoi(properties[0]), stoi(properties[1]));
				int type = stoi(properties[2]);
				createPoleTile(tileset, pos, type);
			}
		}
	}
}

void Entity::createStaticTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect){
	bool canPlace = true;
	for (int i = 0; i < StaticTiles.size(); i++) {
		if (StaticTiles[i].s.getPosition() == pos) {
			canPlace = false;
		}
	}
	if (canPlace) {
		StaticTile tile(tileset, pos, rect);
		StaticTiles.push_back(tile);
		AllTiles.push_back(tile.s);
	}
}

void Entity::destroyStaticTile(sf::Vector2f pos) {
	for (int o = 0; o < StaticTiles.size(); o++) {
		if (pos == StaticTiles[o].s.getPosition()) {
			StaticTiles.erase(StaticTiles.begin() + o);
			break;
		}
	}
	for (int o = 0; o < AllTiles.size(); o++) {
		if (pos == AllTiles[o].getPosition()) {
			AllTiles.erase(AllTiles.begin() + o);
			break;
		}
	}
}

void Entity::destroyBackgroundTile(sf::Vector2f pos) {
	for (int i = 0; i < backgroundTiles.size(); i++) {
		if (backgroundTiles[i].getPosition() == pos) {
			backgroundTiles.erase(backgroundTiles.begin() + i);
			break;
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

	for (int i = 0; i < Poles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(Poles[i].s.getGlobalBounds()) || Debug) {
			window.draw(Poles[i].s);
		}
	}

	window.draw(player.sprite);


	for (int i = 0; i < Enemies.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(Enemies[i].s.getGlobalBounds()) || Debug) {
			window.draw(Enemies[i].s);
		}
	}
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(CoinTiles[i].s);
		}
	}

	for (int i = 0; i < StaticTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(StaticTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(StaticTiles[i].s);
		}
	}
	if (Debug) {
		for (int i = 0; i < CoinTiles.size(); i++) {
			if (player.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || Debug) {
				window.draw(CoinTiles[i].s);
			}
		}

		for (int i = 0; i < transportTiles.size(); i++) {
			if (player.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || Debug) {
				window.draw(transportTiles[i].s);
			}
		}
	}
	if (Debug) {
		for (int i = 0; i < deathTriggers.size(); i++) {
			window.draw(deathTriggers[i]);
		}
		for (int i = 0; i < pickupTiles.size(); i++) {
			window.draw(pickupTiles[i].sprite);
		}
	}
	for (int i = 0; i < FireBalls.size(); i++) {
		window.draw(FireBalls[i].s);
	}

	for (int i = 0; i < particles.size(); i++) {
		window.draw(particles[i].s);
	}
	if (player.dead) {
		window.draw(player.sprite);
	}
	//window.draw(player.areaBounds);
	//window.draw(Mario.hitbox);
	for (int i = 0; i < castleTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(castleTiles[i].getGlobalBounds()) || Debug) {
			window.draw(castleTiles[i]);
		}
	}
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

	for (int i = 0; i < Poles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(Poles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(Poles[i].s.getGlobalBounds()) || Debug) {
			window.draw(Poles[i].s);
		}
	}

	window.draw(player.sprite);
	window.draw(player2.sprite);

	for (int i = 0; i < Enemies.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(Enemies[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(Enemies[i].s.getGlobalBounds()) || Debug) {
			window.draw(Enemies[i].s);
		}
	}

	for (int i = 0; i < castleTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(castleTiles[i].getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(castleTiles[i].getGlobalBounds()) || Debug) {
			window.draw(castleTiles[i]);
		}
	}
	for (int i = 0; i < CoinTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(CoinTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(CoinTiles[i].s);
		}
	}

	for (int i = 0; i < StaticTiles.size(); i++) {
		if (player.areaBounds.getGlobalBounds().intersects(StaticTiles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(StaticTiles[i].s.getGlobalBounds()) || Debug) {
			window.draw(StaticTiles[i].s);
		}
	}
	if (Debug) {
		for (int i = 0; i < transportTiles.size(); i++) {
			if (player.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || player2.areaBounds.getGlobalBounds().intersects(transportTiles[i].s.getGlobalBounds()) || Debug) {
				window.draw(transportTiles[i].s);
			}
		}
	}
	for (int i = 0; i < FireBalls.size(); i++) {
		window.draw(FireBalls[i].s);
	}
	for (int i = 0; i < particles.size(); i++) {
		window.draw(particles[i].s);
	}
	//if (debugMode) {
		for (int i = 0; i < deathTriggers.size(); i++) {
			window.draw(deathTriggers[i]);
		//}
	}
	if (player.dead) {
		window.draw(player.sprite);
	}
	if (player2.dead) {
		window.draw(player2.sprite);
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
			break;
		}
	}
	for (int i = 0; i < AllTiles.size(); i++) {
		if (AllTiles[i].getPosition() == pos) {
			AllTiles.erase(AllTiles.begin() + i);
			break;
		}
	}
}

void Entity::Logic(Player& Mario) {
	for (int i = 0; i < CoinTiles.size(); i++) {
		bool destroyCoin = CoinTiles[i].Logic(Mario, coinCollected, Enemies);
		if (destroyCoin && CoinTiles[i].type == 1) {
			destroyCoinTile(CoinTiles[i].s.getPosition());
		}
	}
	for (int i = 0; i < transportTiles.size(); i++) {
		transportTiles[i].Logic(Mario, oneTime, enterPipeSound);
	}
	for (int i = 0; i < StaticTiles.size(); i++) {
		sf::FloatRect MRect = Mario.hitbox.getGlobalBounds();
		MRect.top -= 2;
		int in = StaticTiles[i].logic(Mario, MRect, Enemies);
		if (in == 1 && StaticTiles[i].s.getTextureRect().left != 99) {
			bool canBreak = true;
			for (int p = 0; p < pickupTiles.size(); p++) {
				if (pickupTiles[p].sprite.getPosition() == StaticTiles[i].s.getPosition()) {
					canBreak = false;
				}
			}
			if (canBreak) {
				createParticle(StaticTiles[i].s.getPosition() + sf::Vector2f(0, 0), sf::Vector2f(-1, -4), Tileset, StaticTiles[i].s.getTextureRect());
				createParticle(StaticTiles[i].s.getPosition() + sf::Vector2f(16, 0), sf::Vector2f(1, -4), Tileset, StaticTiles[i].s.getTextureRect());
				createParticle(StaticTiles[i].s.getPosition() + sf::Vector2f(16, 16), sf::Vector2f(1, 0), Tileset, StaticTiles[i].s.getTextureRect());
				createParticle(StaticTiles[i].s.getPosition() + sf::Vector2f(0, 16), sf::Vector2f(-1, 0), Tileset, StaticTiles[i].s.getTextureRect());
				for (int p = 0; p < AllTiles.size(); p++) {
					if (StaticTiles[i].s.getPosition() == AllTiles[p].getPosition()) {
						AllTiles.erase(AllTiles.begin() + p);
					}
				}
				StaticTiles.erase(StaticTiles.begin() + i);
				breakBlockSound.play();
			}
			else {
				sf::IntRect r = StaticTiles[i].s.getTextureRect();
				r.left = 99;
				cout << "yes";
				StaticTiles[i].s.setTextureRect(r);
			}
		}
	}
	for (int i = 0; i < pickupTiles.size(); i++) {
		bool destroyPickup = pickupTiles[i].Logic(Mario, AllTiles, oneTime, mushCollected, powerUpAppearsSound);
		if (destroyPickup) {
			destroyPickupTile(pickupTiles[i].sprite.getPosition());
		}
	}
	if (!debugMode) {
		for (int i = 0; i < Enemies.size(); i++) {
			if (Enemies[i].s.getGlobalBounds().intersects(Mario.areaBounds.getGlobalBounds())) {
				if (Enemies[i].Logic(Mario, oneTime, AllTiles, Enemies, gumbaDeadSound)) {
					Enemies.erase(Enemies.begin()+i);
				}
			}
			else {
				if (Enemies[i].activated == 2 && Enemies[i].type == 1) {
					Enemies.erase(Enemies.begin() + i);
				}
			}
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		if (oneTime) {
			particles[i].Logic();
		}
	}

	for (int i = 0; i < deathTriggers.size(); i++) {
		if (deathTriggers[i].getGlobalBounds().intersects(Mario.hitbox.getGlobalBounds()) && Mario.finished == 0) {
			if (!Mario.dead) {
				Mario.dead = true;
				//Mario.MoveLockState = true;
			}
		}
	}
	for (int i = 0; i < FireBalls.size(); i++) {
		if (FireBalls[i].s.getGlobalBounds().intersects(Mario.areaBounds.getGlobalBounds())) {
			bool F = FireBalls[i].Logic(Enemies, AllTiles, oneTime);
			if (F) {
				FireBalls.erase(FireBalls.begin() + i);
			}
		}
		else {
			FireBalls.erase(FireBalls.begin() + i);
		}
	}

	for (int i = 0; i < Poles.size(); i++) {
		for (int Po = 0; Po < Poles.size(); Po++) {
			if (Poles[Po].side == 4) {
				Poles[i].Logic(Mario);
			}
		}
	}

	float yThreshold = 0;
	for (int Po = 0; Po < Poles.size(); Po++) {
		if (Poles[Po].side == 1) {
			yThreshold = Poles[Po].s.getPosition().y;
		}
	}
	if (Mario.finished != 0) {
		if (Mario.finishedOnce) {
			Mario.finalCamPos = sf::Vector2f(Mario.sprite.getPosition().x + 110, Mario.Camera.getCenter().y);
			Mario.finishedOnce = false;
		}
		Mario.Camera.move((Mario.finalCamPos.x - Mario.Camera.getCenter().x) / 50, 0);

		for (int Po = 0; Po < Poles.size(); Po++) {
			if (Poles[Po].side == 4) {
				if (Poles[Po].s.getPosition().y < yThreshold) {
					Poles[Po].s.move(0, 1);
				}
			}
		}
	}
	if (Mario.finished == 1) {
		Mario.sprite.setScale(-1, 1);
		Mario.Animate("pole");
		Mario.sprite.setPosition(Poles[0].s.getPosition().x + 2, Mario.sprite.getPosition().y);
		Mario.grav = 0;
		Mario.vel = sf::Vector2f();
		if (Mario.finishedWaitTimer.getElapsedTime().asSeconds() > 0.7) {
			Mario.sprite.move(0, 1);
		}
	}
	else if (Mario.finished == 2) {
		if (Mario.finishedWaitTimer.getElapsedTime().asSeconds() > 1) {
			Mario.sprite.move(1, 1);
			for (int i = 0; i < castleTiles.size(); i++) {
				if (Mario.sprite.getGlobalBounds().left > castleTiles[i].getGlobalBounds().left){
					Mario.finished = 3;
					return;
				}
			}
			Mario.Collision(AllTiles, sf::Vector2f(0, 1));
			Mario.Animate("run");
			Mario.sprite.setScale(-1, 1);
		}
	}
	oneTime = false;
}

void Entity::convertToBackground(sf::Sprite s) {
	bool canPlace = true;
	for (int i = 0; i < backgroundTiles.size(); i++) {
		if (s.getPosition() == backgroundTiles[i].getPosition()) {
			canPlace = false;
		}
	}
	if (canPlace) {
		backgroundTiles.push_back(s);
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
		if (pos == transportTiles[i].s.getPosition()) {
			transportTiles.erase(transportTiles.begin() + i);
			break;
		}
	}
}
void pipeTransportTile::Logic(Player& Mario, bool oneTime, sf::Sound& Sound) {
	if (s.getGlobalBounds().intersects(Mario.sprite.getGlobalBounds())) {
		if (s.getGlobalBounds().left <= Mario.hitbox.getGlobalBounds().left && Mario.sprite.getGlobalBounds().left+16 <= s.getGlobalBounds().left + 16) {
			if (!active && !Mario.MoveLockState) {
				if (((sf::Keyboard::isKeyPressed(sf::Keyboard::S) && Mario.playerNum == 0)|| sf::Joystick::getAxisPosition(Mario.playerNum, sf::Joystick::Y) > Mario.axisThreshold) && dir.y == 1 ||
					((sf::Keyboard::isKeyPressed(sf::Keyboard::D) && Mario.playerNum == 0) || (sf::Joystick::getAxisPosition(Mario.playerNum, sf::Joystick::X) > Mario.axisThreshold)) && dir.x == 1) {
					active = true;
					Sound.play();
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
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i].moved = false;
	}
}

void Entity::createEnemy(sf::Vector2f position, int t) {
	bool canPlace = true;
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i].s.getPosition() == position) {
			canPlace = false;
		}
	}
	if (canPlace) {
		Enemy en(EnemySpritesheet, position, t, debugMode);
		Enemies.push_back(en);
	}
}

Particle::Particle(sf::Vector2f position, sf::Vector2f vel, sf::Texture* TileSet, sf::IntRect TextRect) {
	s.setRadius(4);
	s.setPosition(position);
	velocity = vel;
	s.setTexture(TileSet);
	s.setTextureRect(TextRect);
}

void Particle::Logic() {
	velocity.y += 0.3;
	s.setRotation(s.getRotation() - (s.getRotation() - (180 * velocity.x)*0.2));
	s.move(velocity);
}

void Entity::createParticle(sf::Vector2f position, sf::Vector2f vel, sf::Texture* TileSet, sf::IntRect TextRect) {
	Particle p(position, vel, TileSet, TextRect);
	particles.push_back(p);
}

void Entity::createDeathTrigger(sf::Vector2f pos) {
	sf::RectangleShape s;
	s.setPosition(pos);
	s.setSize(sf::Vector2f(16, 16));
	s.setFillColor(sf::Color(255, 0, 0, 100));
	bool canPlace = true;
	for (int i = 0; i < deathTriggers.size(); i++) {
		if (deathTriggers[i].getPosition() == pos) {
			canPlace = false;
		}
	}
	if (canPlace) {
		deathTriggers.push_back(s);
	}
}

void Entity::destroyDeathTrigger(sf::Vector2f pos) {
	for (int i = 0; i < deathTriggers.size(); i++) {
		if (deathTriggers[i].getPosition() == pos) {
			deathTriggers.erase(deathTriggers.begin() + i);
			break;
		}
	}
}

FireBall::FireBall(sf::Texture& spriteSheet, sf::Vector2f pos, int d) {
	s.setTexture(spriteSheet);
	s.setTextureRect(sf::IntRect(26, 150, 8, 8));
	s.setOrigin(4, 4);
	s.scale(d, 1);
	s.setPosition(pos+sf::Vector2f(d*5, 0));
	dir = d;
	cout << "yes";
}

bool FireBall::Logic(vector<Enemy>& Enemies, vector<sf::Sprite>& AllTiles, bool oneTime) {
	if (oneTime) {
		grav += 0.3;

		if (deathTimer.getElapsedTime().asSeconds() > 10) {
			return true;
		}

		s.move(dir * 3, 0);
		if (collision(dir * 3, 0, AllTiles)) {
			return true;
		}

		if (frameCounter.getElapsedTime().asSeconds() > 0.1) {
			if (s.getTextureRect() == sf::IntRect(26, 150, 8, 8)) {
				s.setTextureRect(sf::IntRect(41, 150, 8, 8));
			}
			else if (s.getTextureRect() == sf::IntRect(41, 150, 8, 8)) {
				s.setTextureRect(sf::IntRect(26, 165, 8, 8));
			}
			else if (s.getTextureRect() == sf::IntRect(26, 165, 8, 8)) {
				s.setTextureRect(sf::IntRect(41, 165, 8, 8));
			}
			else if (s.getTextureRect() == sf::IntRect(41, 165, 8, 8)) {
				s.setTextureRect(sf::IntRect(26, 150, 8, 8));
			}
			frameCounter.restart();
		}

		s.move(0, grav);
		collision(0, grav, AllTiles);
	}
	for (int i = 0; i < Enemies.size(); i++) {
		if (s.getGlobalBounds().intersects(Enemies[i].s.getGlobalBounds())) {
			if (Enemies[i].activated != -1) {
				Enemies[i].grav = -5;
				Enemies[i].activated = -1;
				return true;
			}
		}
	}
	return false;
}

bool FireBall::collision(float velx, float vely, vector<sf::Sprite>& AllTiles) {
	for (int i = 0; i < AllTiles.size(); i++) {
		if (s.getGlobalBounds().intersects(AllTiles[i].getGlobalBounds())) {
			sf::FloatRect selfBounds = s.getGlobalBounds();
			sf::FloatRect collisionBounds = AllTiles[i].getGlobalBounds();
			if (velx > 0) {
				dir = -1;
				s.setPosition(collisionBounds.left - (selfBounds.width / 2), s.getPosition().y);
				return true;
			}
			if (velx < 0) {
				dir = 1;
				s.setPosition(collisionBounds.left + collisionBounds.width + (selfBounds.width / 2), s.getPosition().y);
				return true;
			}
			if (vely > 0) {
				grav = -3;
				s.setPosition(s.getPosition().x, collisionBounds.top - (selfBounds.height / 2));
			}
		}
	}
	return false;
}

void Entity::createFireEntity(sf::Vector2f pos, int dir) {
	FireBall f(EnemySpritesheet, pos, dir);
	FireBalls.push_back(f);
}

PoleTile::PoleTile(sf::Texture& tileSet, sf::Vector2f pos, int e) {
	s.setTexture(tileSet);
	s.setScale(0.5, 0.5);
	side = e;
	if (side == 0 || side == 1) {
		s.setTextureRect(sf::IntRect(528, 297, 32, 32));
	}
	else if (side == 3) {
		s.setTextureRect(sf::IntRect(528, 264, 32, 32));
	}
	else {
		s.setTextureRect(sf::IntRect(363, 660, 32, 32));
	}
	s.setPosition(pos);
}

void PoleTile::Logic(Player& Mario) {
	if (Mario.hitbox.getGlobalBounds().intersects(s.getGlobalBounds())) {
		Mario.MoveLockState = true;
		if (Mario.finished == 0) {
			Mario.finishedWaitTimer.restart();
			Mario.sprite.setPosition(s.getPosition().x + 2, Mario.sprite.getPosition().y);
			Mario.finished = 1;
		}
	}
	if (Mario.finished == 1 && side == 1) {
		if (Mario.sprite.getGlobalBounds().intersects(s.getGlobalBounds())) {
			if ((Mario.sprite.getGlobalBounds().top + 2 > s.getGlobalBounds().top && Mario.smallMario) || (Mario.sprite.getGlobalBounds().top + 20 > s.getGlobalBounds().top && !Mario.smallMario)) {
				Mario.sprite.move(12, 0);
				Mario.sprite.setScale(1, 1);
				Mario.finished = 2;
				Mario.finishedWaitTimer.restart();
			}
		}
	}
}

void Entity::createPoleTile(sf::Texture& tileSet, sf::Vector2f pos, int side) {
	bool canPlace = true;
	for (int i = 0; i < Poles.size(); i++) {
		if (pos == Poles[i].s.getPosition()) {
			canPlace = false;
		}
	}
	if (canPlace) {
		PoleTile p(tileSet, pos, side);
		Poles.push_back(p);
	}
}

void Entity::createCastleTile(sf::Texture& tileset, sf::Vector2f pos, sf::IntRect rect) {
	bool canPlace = true;
	for (int o = 0; o < castleTiles.size(); o++) {
		if (castleTiles[o].getPosition() == pos) {
			canPlace = false;
		}
	}
	if (canPlace) {
		StaticTile s(tileset, pos, rect);
		castleTiles.push_back(s.s);
	}
}

void Entity::destroyCastleTile(sf::Vector2f pos) {
	for (int o = 0; o < castleTiles.size(); o++) {
		if (pos == castleTiles[o].getPosition()) {
			castleTiles.erase(castleTiles.begin() + o);
			break;
		}
	}
}