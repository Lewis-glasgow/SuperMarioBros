#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Player.h"
#include "TileEditor.h"
#include "TileEntity.h"

using namespace std;

void Level(sf::RenderWindow& window, sf::Texture& Tileset, string& MapName, int Mode, int& Lives) {
	Player Mario(window, "assets/mario.png", 0);
	Player Luigi(window, "assets/luigi.png", 1);

	sf::Color backgroundColor(93, 148, 251);
	Entity EntityList;
	EntityList.LoadFromFile("worlds/" + MapName + ".wd" , Tileset, backgroundColor, Mario, Luigi, EntityList);


	if (Mode == 2) {
		window.setSize(sf::Vector2u(window.getSize().x * 2, window.getSize().y));
		window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 8, sf::VideoMode::getDesktopMode().height / 8));
		Mario.Camera.setViewport(sf::FloatRect(0, 0, 0.5, 1));
		Luigi.Camera.setViewport(sf::FloatRect(0.5, 0, 0.5, 1));
	}

	sf::Clock fpsClock;

	sf::Music music;
	music.openFromFile("assets/audio/" + MapName + ".wav");
	music.setLoop(true);
	music.play();

	sf::Font Font;
	Font.loadFromFile("assets/font.ttf");
	sf::Text text;
	text.setFont(Font);

	string Score = "000000";
	int TimeLeft = 300;
	sf::Clock Counter;

	sf::Sprite coinDisplay;
	sf::Texture coinDisplayT;
	sf::Text coinDisplayText;
	coinDisplayT.loadFromFile("assets/menu.png");
	coinDisplay.setTexture(coinDisplayT);
	coinDisplay.setTextureRect(sf::IntRect(264, 76, 8, 8));
	coinDisplay.setScale(3, 3);
	coinDisplay.setPosition(232+20, 47);
	coinDisplayText.setFont(Font);
	coinDisplayText.setPosition(256+20, 42);
	coinDisplayText.setScale(0.8, 0.8);

	sf::Text TimeText;


	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		int fps = 1 / fpsClock.restart().asSeconds();
		if (Counter.getElapsedTime().asSeconds() > 1) {
			TimeLeft -= 1;
			Counter.restart();
		}
		EntityList.resetLogic();
		Mario.Logic(EntityList.AllTiles, window.hasFocus());
		if (Mario.action == "fire") {
			EntityList.createFireEntity(Mario.sprite.getPosition(), -Mario.sprite.getScale().x);
		}
		EntityList.Logic(Mario);

		if (Mode == 2) {
			Luigi.Logic(EntityList.AllTiles, window.hasFocus());
			if (Luigi.action == "fire") {
				EntityList.createFireEntity(Luigi.sprite.getPosition(), -Luigi.sprite.getScale().x);
			}
			EntityList.Logic(Luigi);
		}

		window.clear(backgroundColor);

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

		if (Mode == 1) {
			if (Mario.dead) {
				music.stop();
				if (Mario.switchScreen) {
					Lives -= 1;
					break;
				}
			}
		}
		else {
			if (Mario.dead && Luigi.dead) {
				music.stop();
				if (Mario.switchScreen && Luigi.switchScreen) {
					Lives -= 1;
					break;
				}
			}
		}

		if (TimeLeft <= 0) {
			Luigi.dead = true;
			Mario.dead = true;
			TimeLeft = 0;
		}
	
		string score;
		if (Mode == 2) {
			text.setPosition((int)OnScreenElements.getSize().x/2, (int)10);
			text.setString("  Luigu          World    Time");
			window.draw(text);

			text.setPosition((int)OnScreenElements.getSize().x / 2, (int)text.getGlobalBounds().height + text.getGlobalBounds().top);
			string MP = MapName;
			score = to_string(Luigi.score);
			while (score.length() < 6) {
				score = "0" + score;
			}
			text.setString("  " + score + "         " + MP + "      " );
			window.draw(text);

			window.draw(coinDisplay);
			coinDisplayText.setString("x" + to_string(Luigi.coins));
			coinDisplayText.setPosition(((int)OnScreenElements.getSize().x / 2)+256 + 20, 42);
			coinDisplay.setPosition(((int)OnScreenElements.getSize().x / 2)+232 + 20, 47);
			window.draw(coinDisplayText);
		}

		text.setPosition((int)0, (int)10);
		text.setString("  Mario          World    Time");
		window.draw(text);

		text.setPosition((int)0, (int)text.getGlobalBounds().height+ text.getGlobalBounds().top);
		string MP = MapName;
		score = to_string(Mario.score);
		while (score.length() < 6) {
			score = "0" + score;
		}
		text.setString("  " +score+"         "+MP+"      ");
		window.draw(text);

		window.draw(coinDisplay);
		coinDisplayText.setString("x" + to_string(Mario.coins));
		coinDisplayText.setPosition(256 + 20, 42);
		coinDisplay.setPosition(232 + 20, 47);
		window.draw(coinDisplayText);

		TimeText.setFont(Font);
		TimeText.setString(to_string(TimeLeft));
		TimeText.setOrigin(TimeText.getGlobalBounds().width / 2, TimeText.getGlobalBounds().height / 2);
		TimeText.setPosition((int)659, (int)56);
		window.draw(TimeText);
		if (Mode == 2) {
			TimeText.setPosition(((int)OnScreenElements.getSize().x / 2)+(int)659, (int)56);
			window.draw(TimeText);
		}

		window.display();

		if (Mode == 1) {
			if (Mario.finished == 3) {
				if (MapName == "1-1") {
					MapName = "1-2";
				}
				break;
			}
		}
		else {
			if ((Mario.finished == 3 || Mario.dead) && (Luigi.finished == 3 || Luigi.dead)) {
				if (MapName == "1-1") {
					MapName = "1-2";
				}
			}
		}
	}
}

void LevelEditor(string& MapName) {
	TileEditor Editor;
	Editor.Logic(MapName);
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

	sf::Sprite coin;
	coin.setTexture(Texture);
	coin.setTextureRect(sf::IntRect(264, 76, 8, 8));
	coin.setPosition(88, 8);
	sf::Clock coinFrameCounter;
	int coinAnim = 0;

	int CurserY = 131;
	sf::Clock buttonTime;
	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
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
		if (window.hasFocus()){
			if (CurserY == 131) {
				Mode = 1;
				break;
			}
			else {
				Mode = 2;
				break;
			}
		}
	}

	if (coinFrameCounter.getElapsedTime().asSeconds() > 0.2) {
		coinAnim += 1;
		coinFrameCounter.restart();
	}

	if (coinAnim == 0) {
		coin.setTextureRect(sf::IntRect(264, 76, 8, 8));
	} 
	else if (coinAnim == 1) {
		coin.setTextureRect(sf::IntRect(274, 76, 8, 8));
	}
	else if (coinAnim == 2){
		coin.setTextureRect(sf::IntRect(284, 76, 8, 8));
	}
	else if (coinAnim == 3) {
		coin.setTextureRect(sf::IntRect(264, 76, 8, 8));
	}
	else if (coinAnim >= 5) {
		coinAnim = 0;
	}

	Curser.setPosition(sf::Vector2f(80, CurserY));
	window.clear();
	window.setView(window.getDefaultView());
	window.draw(menu);
	window.draw(Mario);
	window.draw(Curser);

	window.draw(coin);
	window.display();
	}
}

void startScreen(sf::RenderWindow& window, int& Lives, string MapName) {
	sf::Clock clock;
	sf::Texture backgroundT;
	backgroundT.loadFromFile("assets/menu.png");
	sf::Sprite background;
	background.setTexture(backgroundT);
	background.setTextureRect(sf::IntRect(0, 381, 256, 230));

	sf::Font LivesT;
	LivesT.loadFromFile("assets/font.ttf");
	sf::Text LivesText;
	LivesText.setScale(2, 2);
	LivesText.setFont(LivesT);
	LivesText.setString(to_string(Lives));
	LivesText.setPosition(513, 480);

	sf::Text MP;
	MP.setFont(LivesT);
	MP.setString(MapName);
	MP.setPosition(541, 354);

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		if (clock.getElapsedTime().asSeconds() > 1.3) {
			break;
		}


		window.clear();
		window.setView(window.getDefaultView());
		window.draw(background);

		sf::View v;
		window.setView(v);

		window.draw(LivesText);
		MP.setPosition(541, 354);
		window.draw(MP);

		MP.setPosition(590, 108);
		window.draw(MP);
		window.display();

	}
}

void GameOver(sf::RenderWindow& window, string MapName, string Score, int& breakCode) {
	sf::Clock Timer;
	sf::SoundBuffer gameOverBuffer;
	gameOverBuffer.loadFromFile("assets/audio/gameOver.wav");
	sf::Sound gameOverSound;
	gameOverSound.setBuffer(gameOverBuffer);
	gameOverSound.play();

	sf::Font font;
	font.loadFromFile("assets/font.ttf");
	sf::Text text;
	text.setFont(font);

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		sf::View v;
		window.setView(v);

		text.setPosition((int)0, (int)10);
		text.setString("  Mario               World      Time");
		window.draw(text);

		text.setPosition((int)0, (int)text.getGlobalBounds().height + text.getGlobalBounds().top);
		string MP = MapName;
		text.setString("  " + Score + "              " + MP);
		window.draw(text);

		text.setString("GAME OVER");
		text.setPosition((v.getSize().x / 2) - (text.getGlobalBounds().width / 2), (v.getSize().y / 2) - (text.getGlobalBounds().height / 2));
		window.draw(text);

		if (Timer.getElapsedTime().asSeconds() > 7) {
			breakCode = 1;
			break;
		}

		window.display();
	}
}

void setWindowProps(sf::RenderWindow& window) {
	//just for debug
	sf::View v;
	v.setSize(256, 224);
	v.setCenter(256 / 2, 224 / 2);
	window.setView(v);
	window.setSize(sf::Vector2u(256 * 3, 224 * 3));
	window.setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width / 2)-(window.getSize().x/2), (sf::VideoMode::getDesktopMode().height / 2)-(window.getSize().y/2)-20));
}

int main() {
	//initializing assets
	sf::Texture Tileset;
	Tileset.loadFromFile("assets/tileset.png");

	string MapName = "1-1";
	LevelEditor(MapName);

	sf::RenderWindow window(sf::VideoMode(256, 224), "Super Mario Bros");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	setWindowProps(window);

	int breakCode = 0;

	while (window.isOpen()) {
		int Mode;
		int Lives = 3;
		breakCode = 0;

		MainMenu(window, Mode);
		while (window.isOpen() && breakCode != 1) {
			setWindowProps(window);
			if (Lives > 0) {
				startScreen(window, Lives, MapName);
				Level(window, Tileset, MapName, Mode, Lives);
			}
			else {
				GameOver(window, MapName, "000000", breakCode);
			}
		}
	}

	return 0;
}