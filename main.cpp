#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include "TileEditor.h"
#include "TileEntity.h"

using namespace std;

void Level(sf::RenderWindow& window, sf::Texture& Tileset, string MapName, int Mode) {
	Entity EntityList;
	EntityList.LoadFromFile("worlds/" + MapName + ".wd" , Tileset);

	Player Mario(window, "assets/mario.png", 0);
	Player Luigi(window, "assets/luigi.png", 1);

	if (Mode == 2) {
		window.setSize(sf::Vector2u(window.getSize().x * 2, window.getSize().y));
		window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 8, sf::VideoMode::getDesktopMode().height / 8));
		Mario.Camera.setViewport(sf::FloatRect(0, 0, 0.5, 1));
		Luigi.Camera.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));
	}

	sf::Clock fpsClock;

	sf::Music music;
	music.openFromFile("assets/audio/" + MapName + ".wav");
	music.play();

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		int fps = 1 / fpsClock.restart().asSeconds();
		cout << fps << endl;

		EntityList.resetLogic();
		Mario.Logic(EntityList.AllTiles, window.hasFocus());
		EntityList.Logic(Mario);

		if (Mode == 2) {
			Luigi.Logic(EntityList.AllTiles, window.hasFocus());
			EntityList.Logic(Luigi);
		}

		window.clear(sf::Color(93, 148, 251));

		window.setView(Mario.Camera);
		if (Mode == 1) {
			EntityList.DrawToScreen(window, Mario, false);
		}
		else {
			EntityList.DrawToScreen(window, Luigi, Mario, false);
		}

		if (Mode == 2) {
			window.setView(Luigi.Camera);
			EntityList.DrawToScreen(window, Mario, Luigi, false);
		}
		sf::View OnScreenElements;
		OnScreenElements.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
		window.setView(OnScreenElements);

		if (Mode == 2) {
			sf::RectangleShape line;
			line.setSize(sf::Vector2f(4, OnScreenElements.getSize().y));
			line.setPosition((OnScreenElements.getSize().x / 2) - 2, 0);
			line.setFillColor(sf::Color::Black);
			window.draw(line);
		}

		window.display();
	}
}

void LevelEditor(string& MapName) {
	TileEditor Editor;
	Editor.Logic();
}

void MainMenu(sf::RenderWindow& window, int& Mode) {
	sf::Texture Texture;
	Texture.loadFromFile("assets/menu.png");
	sf::Sprite menu;
	menu.setTexture(Texture);
	menu.setPosition(sf::Vector2f(-264, (- 220 / 2 - (224 / 15))-4));

	sf::Texture CurserT;
	CurserT.loadFromFile("assets/menu_curser.png");
	sf::Sprite Curser;
	Curser.setTexture(CurserT);
	Curser.setOrigin(sf::Vector2f(4, 4));

	sf::Texture MarioT;
	MarioT.loadFromFile("assets/mario.png");
	sf::Sprite Mario;
	Mario.setTexture(MarioT);
	Mario.setTextureRect(sf::IntRect(29 + (30 * 5), 30 * 0, 17, 17));
	Mario.setOrigin(sf::Vector2f(8.5, 8.5));
	Mario.setScale(sf::Vector2f(-1, 1));
	Mario.setPosition(sf::Vector2f(256 / 5, 224 - 32 - 8.5 +1));

	int CurserY = 131;
	sf::Clock buttonTime;
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
			//if (window.hasFocus()) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
					sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 50 || sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -50) {

					if (buttonTime.getElapsedTime().asSeconds() > 0.2) {
						if (CurserY == 131) {
							CurserY = 147;
						}
						else {
							CurserY = 131;
						}
						buttonTime.restart();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
					sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -50 || sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 50) {

					if (buttonTime.getElapsedTime().asSeconds() > 0.2) {
						if (CurserY == 131) {
							CurserY = 147;
						}
						else {
							CurserY = 131;
						}
						buttonTime.restart();
					}
				}

			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Joystick::isButtonPressed(0, 1)) {
				if (CurserY == 131) {
					Mode = 1;
					break;
				}
				else {
					Mode = 2;
					break;
				}
			}

			Curser.setPosition(sf::Vector2f(80, CurserY));
			window.clear();
			window.draw(menu);
			window.draw(Mario);
			window.draw(Curser);
			window.display();
		//}
	}
}

int main() {
	string MapName = "level1";
	LevelEditor(MapName);

	sf::RenderWindow window(sf::VideoMode(256, 224), "Super Mario Bros");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	//just for debug
	window.setSize(sf::Vector2u(256 * 3, 224 * 3));
	window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width/4, sf::VideoMode::getDesktopMode().height/4));

	//initializing assets
	sf::Texture Tileset;
	Tileset.loadFromFile("assets/tileset.png");

	while (window.isOpen()) {
		int Mode;
		MainMenu(window, Mode);
		Level(window, Tileset, MapName, Mode);
	}

	return 0;
}