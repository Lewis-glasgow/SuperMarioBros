#include "TileEditor.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "TileEntity.h"
#include <string>
#include "Player.h"

using namespace std;

vector<char> ascii = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

sf::Vector2f snapToGrid(sf::Vector2f pos, sf::View& Camera) {
	sf::Vector2f newPos;

	for (int x = Camera.getCenter().x-350; x < 700 + Camera.getCenter().x + 350; x += 16) {
		if (x <= pos.x && pos.x < Camera.getCenter().x + 350) {
			newPos.x = x;
		}
	}
	for (int y = Camera.getCenter().y - 350; y < 700 + Camera.getCenter().y + 350; y += 16) {
		if (y <= pos.y && pos.y < Camera.getCenter().y + 350) {
			newPos.y = y;
		}
	}
	return newPos;

}

// hotkeys: P-set player pipe to pos (press shift to make it exact to mouse), C-set camera to pos (press shift to make it exact to mouse), T-create pipe (press shift to make it 8px to right), b-convert to background, Ctrl+R-rename world.wd, i-create Pickerable Tile,

void TileEditor::showIcon(sf::RenderWindow& window, sf::Sprite& icon, string& CurrentMode, sf::View& v, sf::Vector2f mousePos, sf::IntRect& TileRect, sf::Texture& TileSet, sf::Texture& EnemySpriteSheet, sf::Texture& MarioSpriteSheet) {
	icon.setPosition(v.getSize().x - 70 + (32 / 2), 10);
	icon.setTextureRect(sf::IntRect(0, 0, 32, 32));
	icon.setScale(1, 1);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "staticTiles";
			TileRect = sf::IntRect(0, 0, 32, 32);
			typeTile = 0;
		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(759, 0, 32, 32));
	icon.setScale(1, 1);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "coins";
			TileRect = sf::IntRect(759, 0, 32, 32);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(33, 660, 32, 32));
	icon.setScale(1, 1);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "background";
			TileRect = sf::IntRect(33, 660, 32, 32);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(0, 264, 64, 64));
	icon.setScale(0.5, 0.5);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "pipe";
			TileRect = sf::IntRect(0, 264, 64, 64);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(0, 4, 16, 16));
	icon.setScale(2, 2);
	icon.setTexture(EnemySpriteSheet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "enemy";
			TileRect = sf::IntRect(0, 4, 16, 16);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(0, 16, 17, 17));
	icon.setScale(2, 2);
	icon.setTexture(MarioSpriteSheet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "deathTrigger";
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(825, 363, 32, 32));
	icon.setScale(1, 1);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "pickupTile";
			TileRect = sf::IntRect(825, 264, 32, 32);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setTextureRect(sf::IntRect(363, 660, 32, 32));
	icon.setScale(1, 1);
	icon.setTexture(TileSet);
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "finishTile";
			TileRect = sf::IntRect(528, 297, 32, 32);
			typeTile = 0;

		}
	}
	window.draw(icon);

	icon.move(0, 42);
	icon.setScale(0.25, 0.25);
	sf::Texture CastleIcon;
	CastleIcon.loadFromFile("assets/castle.png");
	icon.setTexture(CastleIcon);
	icon.setTextureRect(sf::IntRect(0, 0, 160, 160));
	if (icon.getGlobalBounds().intersects(sf::FloatRect(mousePos.x, mousePos.y, 1, 1))) {
		sf::RectangleShape hover;
		hover.setPosition(icon.getPosition().x - 5, icon.getPosition().y - 5);
		hover.setSize(sf::Vector2f(42, 42));
		window.draw(hover);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			CurrentMode = "castleTile";
			TileRect = sf::IntRect(429, 0, 32, 32);
			typeTile = 0;

		}
	}
	icon.move(-4, 0);
	window.draw(icon);
	icon.move(4, 0);
}

string TileEditor::Logic(string Mapname) {

	sf::RenderWindow window(sf::VideoMode(700, 700), "TileEditor");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	sf::View Camera;
	Camera.reset(sf::FloatRect(0, 0, 700, 700));
	float CameraZoom = 1;
	bool camLockState = false;
	bool MovLockState = false;

	sf::Sprite Preview;
	sf::Texture TileSet;
	TileSet.loadFromFile("assets/tileset.png");
	Preview.setTexture(TileSet);
	Preview.setScale(sf::Vector2f(0.5, 0.5));
	Preview.setColor(sf::Color(255, 255, 255, 150));

	sf::RectangleShape marioReference;
	marioReference.setSize(sf::Vector2f(17, 17));
	marioReference.setPosition(sf::Vector2f(256 / 5, 224 - 32 - 8.5));

	float speed = 16.f;
	sf::IntRect tileRect(0, 0, 32, 32);
	Preview.setTextureRect(tileRect);

	Player Mario(window, "assets/mario.png", 0);
	Mario.MoveLockState = true;

	vector<sf::Sprite> Players;
	Players.push_back(Mario.sprite);

	sf::Vector2f ToPlayerPos, ToCameraPos;
	sf::CircleShape PlayerPipePos, CameraPipPos;
	PlayerPipePos.setRadius(10.f);
	CameraPipPos.setRadius(10.f);

	sf::Vector2f pipeDir;
	sf::Text pipeDirText;
	sf::Font pipDirTextFont;
	//pipeDirText.setScale(0.5, 0.5);
	pipDirTextFont.loadFromFile("assets/font.ttf");
	pipeDirText.setFont(pipDirTextFont);

	bool RenamingWorld = false;
	Entity EntityList;
	EntityList.debugMode = true;
	sf::Color backgroundColor(93, 148, 251);
	EntityList.LoadFromFile("worlds/"+MapName, TileSet, backgroundColor, Mario, Mario, EntityList);
	bool RenameOnce = true;
	sf::Text WorldNameText;
	WorldNameText.setFont(pipDirTextFont);

	sf::Clock changeTileTimer;

	sf::RectangleShape uiBar;
	uiBar.setSize(sf::Vector2f(70, 700));
	uiBar.setPosition(680, 0);
	uiBar.setFillColor(sf::Color(180, 180, 170));

	sf::Sprite icon;
	sf::Texture MarioSpriteSheet;
	MarioSpriteSheet.loadFromFile("assets/mario.png");
	icon.setTexture(TileSet);


	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
			if (window.hasFocus()) {
				//zooming
				if (ev.type == sf::Event::MouseWheelMoved) {
					if (ev.mouseWheel.delta > 0) {
						Camera.zoom(0.5);
					}
					else if (ev.mouseWheel.delta < 0) {
						Camera.zoom(2);
					}
				}

				if (ev.type == sf::Event::Resized) {
					Camera.setSize(sf::Vector2f(ev.size.width, ev.size.height));
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
					if (RenameOnce) {
						RenamingWorld = !RenamingWorld;
						RenameOnce = false;
					}
					MapName = "";
				}
				else {
					RenameOnce = true;
				}
				//renaming world
				if (RenamingWorld) {
					if (ev.type == sf::Event::KeyPressed) {
						if (ev.key.code < ascii.size()) {
							MapName += ascii[ev.key.code];
						}

						if (ev.key.code == 59) {
							MapName = MapName.substr(0, MapName.length() - 1);
						}
						if (ev.key.code == 50) {
							MapName += ".";
						}
						if  (ev.key.code == 27){
							MapName += "1";
						}
						if  (ev.key.code == 28){
							MapName += "2";
						}
						if  (ev.key.code == 29){
							MapName += "3";
						}
						if  (ev.key.code == 30){
							MapName += "4";
						}
						if  (ev.key.code == 31){
							MapName += "5";
						}
						if  (ev.key.code == 32){
							MapName += "6";
						}
						if  (ev.key.code == 33){
							MapName += "7";
						}
						if  (ev.key.code == 34){
							MapName += "8";
						}
						if  (ev.key.code == 35){
							MapName += "9";
						}
						if  (ev.key.code == 36){
							MapName += "0";
						}
						if (ev.key.code == 56) {
							MapName += "-";
						}
						if  (ev.key.code == 58){
							EntityList.LoadFromFile("worlds/"+MapName+".wd", TileSet, backgroundColor, Mario, Mario, EntityList);
							RenamingWorld = false;
						}
						//}
					}
				}

				//change block
				if (typeTile < 0) {
					typeTile = 0;
				}
				if (CurrentMode == "staticTiles" || CurrentMode == "background" || CurrentMode == "castleTile") {
					if (changeTileTimer.getElapsedTime().asSeconds() > 0.2) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
							tileRect.top -= 33;
							changeTileTimer.restart();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
							tileRect.top += 33;
							changeTileTimer.restart();
						}
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
							tileRect.left -= 33;
							changeTileTimer.restart();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
							tileRect.left += 33;
							changeTileTimer.restart();
						}

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
							tileRect.width += 16;
							changeTileTimer.restart();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
							tileRect.width -= 16;
							changeTileTimer.restart();
						}

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
							tileRect.height += 16;
							changeTileTimer.restart();
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
							tileRect.height -= 16;
							changeTileTimer.restart();
						}
					}
				}
				else if (CurrentMode == "coins") {
					if (changeTileTimer.getElapsedTime().asSeconds() > 0.2) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
							tileRect.left = 759;
							tileRect.top = 0;
							tileRect.width = 32;
							tileRect.height = 32;
							changeTileTimer.restart();
							typeTile = 0;
						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
							tileRect.left = 759;
							tileRect.top = 33;
							tileRect.width = 32;
							tileRect.height = 32;
							changeTileTimer.restart();
							typeTile = 1;
						}
					}
				}
				else if (CurrentMode == "pipe") {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)) {
						pipeDir.x = 1;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)) {
						pipeDir.x = -1;
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)) {
						pipeDir.y = -1;
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)) {
						pipeDir.y = 1;
					}
				}
				else if (CurrentMode == "enemy") {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
						if (typeTile < 1) {
							typeTile += 1;
						}
						if (typeTile == 1) {
							tileRect = sf::IntRect(180, 0, 16, 23);
						}
						else if (typeTile == 0) {
							tileRect = sf::IntRect(0, 4, 16, 16);
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
						typeTile -= 1;

						if (typeTile == 1) {
							tileRect = sf::IntRect(180, 0, 16, 23);
						}
						else if (typeTile == 0) {
							tileRect = sf::IntRect(0, 4, 16, 16);
						}
					}
				}
				else if (CurrentMode == "pickupTile") {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
						if (typeTile < 1) {
							typeTile += 1;
						}

						if (typeTile == 0) {
							tileRect = sf::IntRect(825, 264, 32, 32);
						}
						else if (typeTile == 1) {
							tileRect = sf::IntRect(825, 330, 32, 32);
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
						typeTile -= 1;
						if (typeTile == 0) {
							tileRect = sf::IntRect(825, 264, 32, 32);
						}
						else if (typeTile == 1) {
							tileRect = sf::IntRect(825, 330, 32, 32);
						}
					}
				}
			}
		}

		if (window.hasFocus() && !RenamingWorld) {
			cout << tileRect.left << " " << tileRect.top << " " << tileRect.width << " " << tileRect.height << endl;
			cout << typeTile << endl;
			//preview block placement
			sf::Vector2i mousePos(sf::Vector2f(sf::Mouse::getPosition(window)));
			sf::Vector2f globalMousePos(window.mapPixelToCoords(mousePos, Camera));
			Preview.setPosition(snapToGrid(globalMousePos, Camera));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canPlace) {
				if (CurrentMode == "staticTiles") {
					EntityList.createStaticTile(TileSet, Preview.getPosition(), tileRect);
				}
				else if (CurrentMode == "coins") {
					if (typeTile == 0) {
						EntityList.createCoinTile(TileSet, Preview.getPosition(), 0);
					}
					else if (typeTile == 1) {
						EntityList.createCoinTile(TileSet, Preview.getPosition(), 1);
					}
				}
				else if (CurrentMode == "background") {
					sf::Sprite s;
					s.setTexture(TileSet);
					s.setTextureRect(tileRect);
					s.setPosition(Preview.getPosition());
					s.setScale(0.5, 0.5);
					EntityList.convertToBackground(s);
				}
				else if (CurrentMode == "pipe") {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							ToPlayerPos = Preview.getPosition();
						}
						else {
							ToPlayerPos = globalMousePos;
						}
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
							ToCameraPos = Preview.getPosition();
						}
						else {
							ToCameraPos = globalMousePos;
						}
					}
					else {
						EntityList.createStaticTile(TileSet, Preview.getPosition(), sf::IntRect(0, 264, 32, 32));
						EntityList.createStaticTile(TileSet, Preview.getPosition() + sf::Vector2f(16, 0), sf::IntRect(33, 264, 32, 32));
						EntityList.createStaticTile(TileSet, Preview.getPosition() + sf::Vector2f(16, 16), sf::IntRect(33, 297, 32, 32));
						EntityList.createStaticTile(TileSet, Preview.getPosition() + sf::Vector2f(0, 16), sf::IntRect(0, 297, 32, 32));
						EntityList.createTransportTile(Preview.getPosition() + sf::Vector2f(8.f, -16.f), sf::Vector2f(16, 16), PlayerPipePos.getPosition(), CameraPipPos.getPosition(), pipeDir);
					}
				}
				else if (CurrentMode == "enemy") {
					if (typeTile == 0) {
						EntityList.createEnemy(Preview.getPosition() + sf::Vector2f(8, 8), 0);
					}
					else if (typeTile == 1) {
						EntityList.createEnemy(Preview.getPosition() + sf::Vector2f(8, 11.5), 1);
					}
				}
				else if (CurrentMode == "deathTrigger") {
					EntityList.createDeathTrigger(Preview.getPosition());
				}
				else if (CurrentMode == "pickupTile") {
					if (typeTile == 0) {
						EntityList.createPickupTile(Preview.getPosition(), 0, TileSet);
					}
					else if (typeTile == 1) {
						EntityList.createPickupTile(Preview.getPosition(), 1, TileSet);
					}
				}
				else if (CurrentMode == "finishTile") {
					EntityList.createPoleTile(TileSet, Preview.getPosition(), 1);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*2), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*3), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*4), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*5), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*6), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*7), 0);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(8, 16*7), 4);
					EntityList.createPoleTile(TileSet, Preview.getPosition()-sf::Vector2f(0, 16*8), 3);
				}
				else if (CurrentMode == "castleTile") {
					EntityList.createCastleTile(TileSet, Preview.getPosition(), tileRect);
				}
			}
			//destroy tiles
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && canPlace) {
				if (CurrentMode == "staticTiles") {
					EntityList.destroyStaticTile(Preview.getPosition());
				}
				else if (CurrentMode == "coins") {
					EntityList.destroyCoinTile(Preview.getPosition());
				}
				else if (CurrentMode == "background") {
					EntityList.destroyBackgroundTile(Preview.getPosition());
				}
				else if (CurrentMode == "pipe") {
					EntityList.destroyStaticTile(Preview.getPosition());
					EntityList.destroyStaticTile(Preview.getPosition() + sf::Vector2f(16, 0));
					EntityList.destroyStaticTile(Preview.getPosition() + sf::Vector2f(16, 16));
					EntityList.destroyStaticTile(Preview.getPosition() + sf::Vector2f(0, 16));
					EntityList.destroyTransportTile(Preview.getPosition() + sf::Vector2f(8.f, -16.f));
				}
				else if (CurrentMode == "enemy") {
					EntityList.destroyEnemy(Preview);
					EntityList.destroyEnemy(Preview);
				}
				else if (CurrentMode == "deathTrigger") {
					EntityList.destroyDeathTrigger(Preview.getPosition());
				}
				else if (CurrentMode == "pickupTile") {
					EntityList.destroyPickupTile(Preview.getPosition());
				}
				else if (CurrentMode == "finishTile") {
					for (int i = 0; i < EntityList.Poles.size(); i++) {
						if (Preview.getPosition() == EntityList.Poles[i].s.getPosition()) {
							EntityList.Poles.erase(EntityList.Poles.begin() + i);
						}
					}
				}
				else if (CurrentMode == "castleTile") {
					EntityList.destroyCastleTile(Preview.getPosition());
				}
			}

			//set preview texture
			if (CurrentMode == "enemy") {
				Preview.setTexture(EntityList.EnemySpritesheet);
				Preview.setScale(1, 1);
			}
			else if (CurrentMode == "deathTrigger") {
				tileRect = sf::IntRect(759, 429, 32, 32);
				Preview.setTexture(TileSet);
				Preview.setScale(0.5, 0.5);
			}
			else {
				Preview.setTexture(TileSet);
				Preview.setScale(0.5, 0.5);
			}
			Preview.setTextureRect(tileRect);

			//controls
			//cout << tileRect.left << " " << tileRect.top << " " << tileRect.width << " " << tileRect.height << " " << endl;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				Camera.move(sf::Vector2f(speed, 0));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				Camera.move(sf::Vector2f(-speed, 0));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				Camera.move(sf::Vector2f(0, -speed));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				Camera.move(sf::Vector2f(0, speed));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				EntityList.saveToFile("worlds/" + MapName);
			}
		}

		//rendering
		PlayerPipePos.setPosition(ToPlayerPos);
		CameraPipPos.setPosition(ToCameraPos);
		EntityList.Logic(Mario);
		window.clear(sf::Color(93, 148, 251));

		window.setView(Camera);
		EntityList.DrawToScreen(window, Mario, true);
		window.draw(PlayerPipePos);
		window.draw(CameraPipPos);
		//window.draw(marioReference);
		window.draw(Preview);

		sf::View v;
		window.setView(v);

		pipeDirText.setString("Pipe Direction " + to_string((int)pipeDir.x) + " , " + to_string((int)pipeDir.y));
		pipeDirText.setPosition(0,0);
		window.draw(pipeDirText);

		pipeDirText.setString("World Name " + MapName);
		pipeDirText.setPosition(0, 50);
		window.draw(pipeDirText);

		uiBar.setPosition(v.getSize().x - 70, 0);
		uiBar.setSize(sf::Vector2f(70, v.getSize().y));
		window.draw(uiBar);

		//icons
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition(window)));
		if (mousePos.x >= v.getSize().x-70) {
			canPlace = false;
		}
		else {
			canPlace = true;
		}
		showIcon(window, icon, CurrentMode, v, mousePos, tileRect, TileSet, EntityList.EnemySpritesheet, MarioSpriteSheet);

		window.display();
	}
	return MapName;
}
