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

string TileEditor::Logic() {

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
	EntityList.LoadFromFile("worlds/"+MapName, TileSet);
	bool RenameOnce = true;
	sf::Text WorldNameText;
	WorldNameText.setFont(pipDirTextFont);

	sf::Clock changeTileTimer;

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
						cout << ev.key.code << endl;
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
						if  (ev.key.code == 58){
							EntityList.LoadFromFile("worlds/"+MapName, TileSet);
							RenamingWorld = false;
						}
						//}
					}
				}

				//change block
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
		}

		if (window.hasFocus() && !RenamingWorld) {
			//preview block placement
			sf::Vector2i mousePos(sf::Vector2f(sf::Mouse::getPosition(window)));
			sf::Vector2f globalMousePos(window.mapPixelToCoords(mousePos, Camera));
			Preview.setPosition(snapToGrid(globalMousePos, Camera));

			//tile placement
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
					EntityList.convertToBackground(Preview.getPosition());
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
						EntityList.createTransportTile(Preview.getPosition() + sf::Vector2f(8.f, 0), sf::Vector2f(16, 16), PlayerPipePos.getPosition(), CameraPipPos.getPosition(), pipeDir);
					}
					else {
						EntityList.createTransportTile(Preview.getPosition(), sf::Vector2f(16, 16), PlayerPipePos.getPosition(), CameraPipPos.getPosition(), pipeDir);
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
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
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
					if (tileRect == sf::IntRect(825, 264, 32, 32)) {
						EntityList.createPickupTile(Preview.getPosition(), 0, TileSet);
					}
				}

				else {
					if (tileRect == sf::IntRect(759, 0, 32, 32)) {
						EntityList.createCoinTile(TileSet, Preview.getPosition(), 0);
					}
					else if (tileRect == sf::IntRect(759, 33, 32, 32)) {
						EntityList.createCoinTile(TileSet, Preview.getPosition(), 1);
					}
					else {
						EntityList.createStaticTile(TileSet, Preview.getPosition(), tileRect);
					}
				}
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				EntityList.destroyStaticTile(Preview.getPosition());
				EntityList.destroyCoinTile(Preview.getPosition());
				EntityList.destroyTransportTile(Preview.getPosition());
			}
			Preview.setTextureRect(tileRect);

			//controls
			cout << tileRect.left << " " << tileRect.top << " " << tileRect.width << " " << tileRect.height << " " << endl;
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
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
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					pipeDir.x = 1;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					pipeDir.x = -1;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					pipeDir.y = -1;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					pipeDir.y = 1;
				}
			}
		}

		//rendering
		PlayerPipePos.setPosition(ToPlayerPos);
		CameraPipPos.setPosition(ToCameraPos);
		EntityList.Logic(Mario);
		window.clear(sf::Color(93, 148, 251));
		window.setView(Camera);

		pipeDirText.setString("Pipe Direction: " + to_string((int)pipeDir.x) + "," + to_string((int)pipeDir.y));
		pipeDirText.setPosition(sf::Vector2f(Camera.getCenter()) - sf::Vector2f(window.getSize()) / 2.f);
		window.draw(pipeDirText);

		pipeDirText.setString("World Name: " + MapName);
		pipeDirText.setPosition(sf::Vector2f((Camera.getCenter()) - sf::Vector2f(window.getSize()) / 2.f)+sf::Vector2f(240,0));
		window.draw(pipeDirText);

		EntityList.DrawToScreen(window, Mario, true);
		window.draw(Preview);
		window.draw(PlayerPipePos);
		window.draw(CameraPipPos);
		window.draw(marioReference);

		window.display();
	}
	return MapName;
}
