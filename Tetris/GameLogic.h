#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Common.h"
#include "AI.h"
#include "Tetromino.h"

#include <vector>
#include <stdio.h>
#include <time.h>


class GameLogic
{
private:

	// variables:

	/* GENETIC ALGORITHM */

	/* define the art of program we're starting */
	/* only one can be true at the same time */

	bool finishedAIplays;
	bool geneticAlgorithmComputing;
	bool singlePlayer;

	int totalGamesPlayed;


	/* ----------------- */


	const int invisibleTopLines = 2;
	const int mapHeight = 22;
	const int mapWidth = 10;
	const int maxLevel = 17;

	std::vector< std::vector<int> > landed;

	Tetromino* currentTet;
	Tetromino* nextTet;
	Tetromino* thirdTet;
	AI* ai;

	int score = 0;
	int level = 0;
	int totalLinesCleared = 0;
	int amountOfTetrominosDropped = 0;

	// for AI only:

	bool gameOver = false;
	bool firmDropped = false;
	std::vector <int> randomBag;


	// functions:	

	void spawningNewTetro();
	void resetGame();

	void calculatingScore(int lines);
	void setGameOver();
	bool checkForGameOver();

	void tetroHasLanded();


public:



	void checkForAIreset();

	int bottomLineTarget = 0;

	// functions: 


	int checkForCompletedLines(std::vector< std::vector<int> > *landedMatrix);
	void clearLine(int index, std::vector< std::vector<int> > *landedMatrix);


	void setAI(AI* ai);

	void firmDrop();
	void rotate(int degreeIndicator);
	void moveRight();
	void moveLeft();
	bool getGameOver();

	void setGamePlay(int i);

	/* GETTERS */

	int getTotalGamesPlayed();
	int getAmountOfTetrominosDropped();

	int getMapHeight();
	int getMapWidth();
	int getScore();
	int getLevel();

	bool getFinishedAIPlays();
	bool getGeneticAlgorithmComputing();
	bool getSinglePlayer();
	

	bool dropAI();

	bool automaticDrop();

	Tetromino* getCurrentTetromino();
	Tetromino* getNextTetromino();
	Tetromino* getThirdTetromino();

	void initializeTetros();

	std::vector< std::vector<int> > getLandedMatrix();


	GameLogic();
	~GameLogic();
};