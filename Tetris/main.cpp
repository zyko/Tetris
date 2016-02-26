#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Common.h"
#include "GameLogic.h"
#include "Tetromino.h"
#include "AI.h"
#include "View.h"

#include <memory>		// this is for multithreading (not implemented)
#include <vector>		// this is for std::vector
#include <windows.h>	// this is for resizing command line
#include <thread>		// this is for multithreading (not implemented)





int main()
{
	sf::RenderWindow window(sf::VideoMode(1260, 860), "Tetris AI");


	#pragma region set cmd size
	COORD c;
	c.X = 200; 
	c.Y = 1000;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), c);
	#pragma endregion

	sf::Clock clock; // starts the clock
	sf::Clock frameRateClock;
	
	bool pause = false;


	GameLogic* gameLogic = new GameLogic();

	AI* ai = new AI(gameLogic);
	gameLogic->setAI(ai);

	View* view = new View(&window);
	view->setGameLogic(gameLogic);
	view->setAI(ai);


	#pragma region audio

	sf::Music music;
	if (!music.openFromFile("Sound/tetris_theme.wav"))
		printf("ERROR while loading tetris_theme");

	music.play();
	music.setVolume(10);
	music.setLoop(true);

	#pragma endregion




	while (window.isOpen())
	{

		#pragma region SFML event stuff + keyboard / mouse events
		sf::Event event;
		while (window.pollEvent(event) )
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == sf::Keyboard::Escape)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && !view->isGamePlayChosen())
				if (event.mouseButton.button == sf::Mouse::Left)
					view->choseGamePlay(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);


			if (event.type == sf::Event::MouseMoved && !view->isGamePlayChosen())
				view->highlightButtons(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);


			if (event.type == sf::Event::KeyPressed && !gameLogic->getGameOver() && gameLogic->getSinglePlayer())
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

		if (view->isGamePlayChosen())
		{
			if (!(gameLogic->getGameOver()))
			{


				view->drawBackground();

				#pragma region trigger automaticDrop

				sf::Time elapsed = clock.getElapsedTime();

				if (!pause)
				{
					if (elapsed.asSeconds() >= 0.1f)
					{
						if (gameLogic->getGeneticAlgorithmComputing())
						{
							clock.restart();

							// returns true, if tetromino has landed. this also triggers spawning new tetrominos within gamelogic.cpp
							if (gameLogic->dropAI())
							{
								// once the tetromino has landed, starting computation for next tetromino
								ai->makeDecision(false);
							}
						}
						else if (gameLogic->getFinishedAIPlays())
						{
							clock.restart();

							// returns true, if tetromino has landed. this also triggers spawning new tetrominos within gamelogic.cpp
							if (gameLogic->automaticDrop())
							{
								// once the tetromino has landed, starting computation for next tetromino
								ai->makeDecision(false);
							}
						}
					}
					if (elapsed.asSeconds() >= 1.f - 0.05f * gameLogic->getLevel())
					{
						if (gameLogic->getSinglePlayer())
						{
							clock.restart();
							gameLogic->automaticDrop();
						}

					}
				}
				else if (pause)
				{
					//drawPauseScreen();
				}


				#pragma endregion

				
				view->drawCurrentTetromino();

				view->drawNextTetro();

				view->drawLandedArray();
				
				

				view->drawInterface();

			}
			else
				view->drawGameOver();
		}
		else
			view->drawGamePlaySelection();
		
		window.display();
	}

	/*delete ai;
	delete gameLogic;
	delete view;*/

	return 0;
}