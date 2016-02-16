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

sf::Sprite tealBlockSprite;
sf::Sprite yellowBlockSprite;
sf::Sprite purpleBlockSprite;
sf::Sprite greenBlockSprite;
sf::Sprite redBlockSprite;
sf::Sprite blueBlockSprite;
sf::Sprite orangeBlockSprite;

sf::Sprite nextTetSprt;

sf::Text levelText;
sf::Text scoreText;

GameLogic* gameLogic;
AI* ai;

unique_ptr<thread> threadAI;

const float tileOffset = 40.f;
const float backgroundTxtOffsetX = 30.f;
const float backgroundTxtOffsetY = -50.f;


sf::RenderWindow window(sf::VideoMode(630, 860), "Tetris AI clone");


void drawNextTetromino()
{
	
}

void drawInterface()
{
	if (gameLogic->getScore() > 10000)
		scoreText.setPosition(sf::Vector2f(460.f, 560.f));
	else if (gameLogic->getScore() > 1000)		  
		scoreText.setPosition(sf::Vector2f(470.f, 560.f));
	else if (gameLogic->getScore() > 100)		  
		scoreText.setPosition(sf::Vector2f(480.f, 560.f));
	else if (gameLogic->getScore() > 10)		  
		scoreText.setPosition(sf::Vector2f(490.f, 560.f));
	

	scoreText.setString(to_string(gameLogic->getScore()));
	levelText.setString(to_string(gameLogic->getLevel()));
	
	window.draw(scoreText);
	window.draw(levelText);
	window.draw(nextTetSprt);

	drawNextTetromino();
}

// draws the current Tetromino with data from gamelogic
void drawCurrentTetro()
{
	if (!gameLogic->getGameOver())
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 1)
				{
					tealBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
															gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(tealBlockSprite);
					
				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 2)
				{
					
					yellowBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
																gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(yellowBlockSprite);
				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 3)
				{
					window.draw(purpleBlockSprite);
					purpleBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
																gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 4)
				{
					greenBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(greenBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 5)
				{
					redBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(redBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 6)
				{
					blueBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(blueBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 7)
				{
					orangeBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
																gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window.draw(orangeBlockSprite);

				}
}

// draws landed array with data from gamelogic
void drawLandedArray()
{
	//printf("drawing landed array is called \n");
	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			if (gameLogic->getLandedMatrix()[row][col] == 1)
			{
				tealBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(tealBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 2)
			{
				yellowBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(yellowBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 3)
			{
				purpleBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(purpleBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 4)
			{
				greenBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(greenBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 5)
			{
				redBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(redBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 6)
			{
				blueBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(blueBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 7)
			{
				orangeBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window.draw(orangeBlockSprite);
			}

}


// multithreading
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
// ------------


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
		if (!backgroundTxt.loadFromFile("Assets/background_630x860.png"))
			printf("error while loading background texture \n");


		sf::Sprite backgroundSprt;
		backgroundSprt.setTexture(backgroundTxt);


		// GAME OVER
		
		sf::Texture gameOverTxt;
		if (!gameOverTxt.loadFromFile("Assets/gameOver.png"))
			printf("error while loading gameOver.png \n");

		sf::Sprite gameOverSprt;
		gameOverSprt.setTexture(gameOverTxt);
		

		sf::Texture redBlockTexture;
		if (!redBlockTexture.loadFromFile("Assets/RedBlock_40x40.png"))
			printf("error while loading redBlockTexture");
		redBlockSprite.setTexture(redBlockTexture);

		sf::Texture blueBlockTexture;
		if (!blueBlockTexture.loadFromFile("Assets/BlueBlock_40x40.png"))
			printf("error while loading blueBlockTexture");
		blueBlockSprite.setTexture(blueBlockTexture);

		sf::Texture greenBlockTexture;
		if (!greenBlockTexture.loadFromFile("Assets/GreenBlock_40x40.png"))
			printf("error while loading greenBlockTexture");
		greenBlockSprite.setTexture(greenBlockTexture);

		sf::Texture tealBlockTexture;
		if (!tealBlockTexture.loadFromFile("Assets/TealBlock_40x40.png"))
			printf("error while loading tealBlockTexture");
		tealBlockSprite.setTexture(tealBlockTexture);

		sf::Texture orangeBlockTexture;
		if (!orangeBlockTexture.loadFromFile("Assets/OrangeBlock_40x40.png"))
			printf("error while loading orangeBlockTexture");
		orangeBlockSprite.setTexture(orangeBlockTexture);

		sf::Texture yellowBlockTexture;
		if (!yellowBlockTexture.loadFromFile("Assets/YellowBlock_40x40.png"))
			printf("error while loading yellowBlockTexture");
		yellowBlockSprite.setTexture(yellowBlockTexture);

		sf::Texture purpleBlockTexture;
		if (!purpleBlockTexture.loadFromFile("Assets/PurpleBlock_40x40.png"))
			printf("error while loading purpleBlockTexture");
		purpleBlockSprite.setTexture(purpleBlockTexture);


	#pragma endregion

	#pragma region texts

		sf::Font font;
		if (!font.loadFromFile("Fonts/courbd.ttf")) // this is courier new bold
		{
			printf("error while loading font \n");
		}
		levelText.setFont(font);
		levelText.setCharacterSize(40);
		levelText.setColor(sf::Color::White);
		levelText.setPosition(sf::Vector2f(500.f, 730.f));


		scoreText.setFont(font);
		scoreText.setCharacterSize(40);
		scoreText.setColor(sf::Color::White);
		scoreText.setPosition(sf::Vector2f(500.f, 560.f));


	#pragma endregion

	// currently disabled
	#pragma region audio

	sf::Music music;
	if (!music.openFromFile("Sound/tetris_theme.wav"))
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
				if (elapsed.asSeconds() >= 0.2f)
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