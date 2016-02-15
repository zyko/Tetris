#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "GameLogic.h"
#include "Tetromino.h"
#include "AI.h"

#include <memory>
#include <vector>
#include <stdio.h>
#include <windows.h>
#include <thread>


using namespace std;

sf::Sprite redBlockSprite;
sf::Sprite nextTetSprt;

sf::Text levelText;
sf::Text scoreText;

GameLogic* gameLogic;
AI* ai;

unique_ptr<thread> threadAI;

const float tileOffset = 40.f;

sf::RenderWindow window(sf::VideoMode(806, 1046), "My window");


void drawInterface()
{
	if (gameLogic->getScore() > 10000)
		scoreText.setPosition(sf::Vector2f(547.f, 655.f));
	else if (gameLogic->getScore() > 1000)
		scoreText.setPosition(sf::Vector2f(555.f, 655.f));
	else if (gameLogic->getScore() > 100)
		scoreText.setPosition(sf::Vector2f(570.f, 655.f));
	else if (gameLogic->getScore() > 10)
		scoreText.setPosition(sf::Vector2f(585.f, 655.f));
	

	scoreText.setString(to_string(gameLogic->getScore()));
	levelText.setString(to_string(gameLogic->getLevel()));
	
	window.draw(scoreText);
	window.draw(levelText);
	window.draw(nextTetSprt);
}

// draws the current Tetromino with data from gamelogic
void drawCurrentTetro()
{
	if (!gameLogic->getGameOver())
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (gameLogic->getCurrentTetromino()->getShape()[row][col] != 0)
				{
					window.draw(redBlockSprite);
					redBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset,
															gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset));
				}
}

// draws landed array with data from gamelogic
void drawLandedArray()
{
	//printf("drawing landed array is called \n");
	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			if (gameLogic->getLandedMatrix()[row][col] != 0)
			{
				window.draw(redBlockSprite);
				redBlockSprite.setPosition(sf::Vector2f(col * tileOffset, row * tileOffset));
			} 
}

void secondThreadAI()
{
	ai->makeDecision(false);
}

void start()
{
	threadAI = std::unique_ptr<std::thread>(new std::thread(secondThreadAI));
}

void stop()
{
	threadAI->join();
}

int main()
{
	#pragma region set cmd size
	COORD c;
	c.X = 200; //  Zeichen/Zeile
	c.Y = 1000; //  Zeilen
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
	#pragma endregion

	sf::Clock clock; // starts the clock
	sf::Clock frameRateClock;
	
	bool pause = false;

	gameLogic = new GameLogic();
	ai = new AI(gameLogic);
	gameLogic->setAI(ai);

	ai->makeDecision(true);
	ai->moveTetromino();
	ai->makeDecision(false);
	//Launch a thread
	//thread threadAI(secondThreadAI);

	bool AIisTurnedOn = true;

	

	/* initialize random seed: */
	srand(time(NULL));


	#pragma region sprites

		// BACKGROUND
		sf::Texture backgroundTxt;
		backgroundTxt.loadFromFile("background.png");

		sf::Sprite backgroundSprt;
		backgroundSprt.setTexture(backgroundTxt);


		// GAME OVER
		
		sf::Texture gameOverTxt;
		gameOverTxt.loadFromFile("gameOver.png");

		sf::Sprite gameOverSprt;
		gameOverSprt.setTexture(gameOverTxt);
		

		// NEXT TETROMINO
		sf::Texture nextTetText;
		if (!nextTetText.loadFromFile("T1.png"))
		{
			printf("ERROR");
		}
		nextTetSprt.setTexture(nextTetText);
		nextTetSprt.setPosition(sf::Vector2f(545.f, 210.f)); // absolute position
		nextTetSprt.rotate(0); // offset relative to the current angle


		// SPRITE FOR LANDED ARRAY
		sf::Texture redBlockTexture;
		if (!redBlockTexture.loadFromFile("Red_Block.png"))
		{
			printf("ERROR");
		}
		redBlockSprite.setTexture(redBlockTexture);


	#pragma endregion

	#pragma region texts

		sf::Font font;
		if (!font.loadFromFile("courbd.ttf")) // should be courier new
		{
			printf("ERROR");
		}
		levelText.setFont(font);
		levelText.setCharacterSize(40);
		levelText.setString("2");
		levelText.setColor(sf::Color::White);
		levelText.setPosition(sf::Vector2f(595.f, 835.f));
	
		scoreText.setFont(font);
		scoreText.setCharacterSize(40);
		scoreText.setString("10000");
		scoreText.setColor(sf::Color::White);
		scoreText.setPosition(sf::Vector2f(595.f, 655.f));

	#pragma endregion

	// currently disabled
	#pragma region audio

	sf::Music music;
	if (!music.openFromFile("tetris_theme.wav"))
		printf("ERROR while loading tetris_theme");
	//music.play();
	music.setVolume(20);
	music.setLoop(true);

	#pragma endregion


	while (window.isOpen())
	{

		#pragma region SFML event stuff + keyboard events
		sf::Event event;
		while (window.pollEvent(event) )
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == sf::Keyboard::Escape)
				window.close();

			if (event.type == sf::Event::KeyPressed && !gameLogic->getGameOver() && !AIisTurnedOn)
			{
				if (event.key.code == sf::Keyboard::Right)
					gameLogic->moveRight();
						//drawCurrentTetro();

				if (event.key.code == sf::Keyboard::Left)
					gameLogic->moveLeft();
						//drawCurrentTetro();

				// hard drop
				if (event.key.code == sf::Keyboard::Down)
					gameLogic->automaticDrop();

				// firm drop
				if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return)
					gameLogic->firmDrop();

				if (event.key.code == sf::Keyboard::F)
					gameLogic->rotate(1); // // 1 = 90°, 3 = 270° / -90°

				if (event.key.code == sf::Keyboard::D)
					gameLogic->rotate(3); // // 1 = 90°, 3 = 270° / -90°	
			}
			if (event.type == sf::Event::KeyReleased && !gameLogic->getGameOver() && !pause)
				if (event.key.code == sf::Keyboard::P)
					pause = true;
			/*if (event.type == sf::Event::KeyReleased && !gameLogic->getGameOver() && pause)
				if (event.key.code == sf::Keyboard::P)
					pause = false;*/
		}
		#pragma endregion
		

		// currently disabled
		#pragma region framerate
		//sf::Time frameRateTime = frameRateClock.getElapsedTime();
		//float framerate = 1.0f / frameRateTime.asSeconds();
		//printf("frameRate: %f \n", framerate );
		//frameRateClock.restart().asSeconds();
		#pragma endregion 

		window.clear();

		if (!(gameLogic->getGameOver()))
		{

			#pragma region drawing background
			window.draw(backgroundSprt);
			#pragma endregion

			

			#pragma region trigger automaticDrop

			sf::Time elapsed = clock.getElapsedTime();

			if (AIisTurnedOn && !pause)
			{
				if (elapsed.asSeconds() >= 0.1f)
				{
					clock.restart();
					//gameLogic->automaticDrop();

					// returns true, if tetromino has landed. this also triggers spawning new tetrominos within gamelogic.cpp
					if ( gameLogic->automaticDrop()) //gameLogic->dropAI()) // <--- might be more performant
					{
						// once the tetromino has landed, starting computation for next tetromino
						ai->makeDecision(false);
						/*
						if (ai->isComputationDone())
							start();
						*/
						//stop();
						
					
					}
							//thread threadAI(secondThreadAI);
				}
			}
			if (!AIisTurnedOn && !pause)
			{
				if (elapsed.asSeconds() >= 1.f - 0.05 * gameLogic->getLevel())
				{
					clock.restart();
					gameLogic->automaticDrop();
				}
				
			}
			else if (pause)
			{
				//drawPauseScreen();
			}
				

			#pragma endregion

			//threadAI.join();

			

			//ai->makeDecision();

			drawCurrentTetro();

			drawLandedArray();

			drawInterface();

		}
		else
			window.draw(gameOverSprt);


		window.display();
	}



	return 0;
}