#include "GameLogic.h"



using namespace std;


GameLogic::GameLogic()
{
	// initializing landed array
	vector <int> tmp;
	for (int i = 0; i < mapWidth; ++i)
		tmp.push_back(0);

	for (int col = 0; col < mapHeight; ++col)
		landed.push_back(tmp);

	randomBag = { 0, 1, 2, 3, 4, 5, 6 };

	initializeTetros();



	// debug stuff

	
	for (int i = 19; i < mapHeight; ++i)
		landed[i][0] = 1;
	for (int i = 17; i < mapHeight; ++i)
		landed[i][1] = 1;
	for (int i = 17; i < mapHeight; ++i)
		landed[i][2] = 1;
	for (int i = 17; i < mapHeight; ++i)
		landed[i][3] = 1;
	for (int i = 16; i < mapHeight; ++i)
		landed[i][4] = 1;
	for (int i = 16; i < mapHeight; ++i)
		landed[i][5] = 1;
	for (int i = 17; i < mapHeight; ++i)
		landed[i][6] = 1;
	for (int i = 18; i < mapHeight; ++i)
		landed[i][7] = 1;
	for (int i = 18; i < mapHeight; ++i)
		landed[i][8] = 1;
	
	landed[20][3] = 0;
	landed[18][3] = 0;

	
	/*
	for (int i = 17; i < mapHeight; ++i)
		landed[i][9] = 1;

	/*
	landed[19][0] = 1;
	landed[17][1] = 1;
	landed[17][2] = 1;
	landed[17][3] = 1;
	landed[16][4] = 1;
	landed[16][5] = 1;
	landed[17][6] = 1;
	landed[18][7] = 1;
	landed[18][8] = 1;
	landed[17][9] = 1;
	*/
}

void GameLogic::initializeTetros()
{
	currentTet = new Tetromino(2);
	nextTet = new Tetromino(2);
	thirdTet = new Tetromino(2);
}

// drop method for AI // obsolet?
bool GameLogic::dropAI()
{
	if (currentTet->topLeft[0] == bottomLineTarget)
	{
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (currentTet->getShape()[row][col] != 0)
					landed[row + currentTet->topLeft[0]][col + currentTet->topLeft[1]] = currentTet->getShape()[row][col];


		tetroHasLanded();
		return true;
	}
	else
		currentTet->topLeft[0]++;

	return false;
}

// this is for normal single player
bool GameLogic::automaticDrop()
{
	// checking for collisions
	vector <int> potentialTopLeft = { currentTet->topLeft[0] + 1, currentTet->topLeft[1] };
	bool landing = false;

	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (currentTet->getShape()[row][col] != 0)
				if (row + potentialTopLeft[0] >= mapHeight)									// either the array is empty and it hits the ground
					landing = true;
				else if (landed[row + potentialTopLeft[0]][col + potentialTopLeft[1]] != 0) // or it hits previous tetros within the landing array
					landing = true;

	// adding current tetromino to landed array
	if (landing)
	{
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (currentTet->getShape()[row][col] != 0)
					landed[row + currentTet->topLeft[0]][col + currentTet->topLeft[1]] = currentTet->getShape()[row][col];

		tetroHasLanded();
		return true;
	}
	else
		currentTet->topLeft[0]++;

	return false;

}

void GameLogic::tetroHasLanded()
{
	firmDropped = true;
	checkForCompletedLines();
	spawningNewTetro();
}

void GameLogic::spawningNewTetro()
{
	if (!gameOver)
	{
		currentTet->~Tetromino();
		currentTet = nextTet;
		nextTet = thirdTet;

		
		ai->moveTetromino();

		/*  spawning after the tetris "rule":
		*	all 7 tetrominos must be spawned, before another same can be spawned
		*/
		if (randomBag.size() <= 0)
			randomBag = { 0, 1, 2, 3, 4, 5, 6 };

		int rndInt = rand() % randomBag.size();

		thirdTet = new Tetromino(randomBag[rndInt]);

		randomBag.erase(randomBag.begin() + rndInt);
	}
}

void GameLogic::moveRight()
{
	bool blocked = false;

	vector <int> potentialTopLeft = { currentTet->topLeft[0], currentTet->topLeft[1] + 1 };

	// check for collisions
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (currentTet->getShape()[row][col] != 0)
			{
		if (col + potentialTopLeft[1] >= mapWidth)
			blocked = true;
		else if (landed[row + potentialTopLeft[0]][col + potentialTopLeft[1]] != 0)
			blocked = true;
			}

	if (!blocked)
		currentTet->topLeft[1]++;
}

void GameLogic::moveLeft()
{
	vector <int> potentialTopLeft = { currentTet->topLeft[0], currentTet->topLeft[1] - 1 };

	bool blocked = false;

	// check for collisions
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (currentTet->getShape()[row][col] != 0)
			{
				if (col + potentialTopLeft[1] < 0)
					blocked = true;
				else if (landed[row + potentialTopLeft[0]][col + potentialTopLeft[1]] != 0)
					blocked = true;
			}

	if (!blocked)
		currentTet->topLeft[1]--;
}

void GameLogic::rotate(int degreeIndicator)
{
	int rotateBack;

	if (degreeIndicator == 3)
		rotateBack = 1;
	else if (degreeIndicator == 1)
		rotateBack = 3;


	currentTet->rotate(degreeIndicator);


	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (currentTet->getShape()[row][col] != 0)
			{	// prevent from moving "out of bounds" / playing area
		if (col + currentTet->topLeft[1] < 0)			// left side
		{
			currentTet->rotate(rotateBack);		// rotating again to 360� 
		}
		else if (col + currentTet->topLeft[1] >= mapWidth)	// right side
		{
			currentTet->rotate(rotateBack);		// rotating again to 360� 
		}
		else if (row + currentTet->topLeft[0] >= mapHeight)	// bottom
		{
			currentTet->rotate(rotateBack);		// rotating again to 360� 
		}
		else if (landed[row + currentTet->topLeft[0]][col + currentTet->topLeft[1]] != 0) // space is already taken
		{
			currentTet->rotate(rotateBack);
		}
			}
}

void GameLogic::clearLine(int index)
{
	int toClear = index;
	for (toClear; toClear >= invisibleTopLines; --toClear)
		for (int col = 0; col < mapWidth; ++col)
			landed[toClear][col] = landed[toClear - 1][col];
}

void GameLogic::calculatingScore(int lines)
{
	totalLinesCleared += lines;
	int lineConstant;

	if (lines == 1)
		lineConstant = 40;
	if (lines == 2)
		lineConstant = 100;
	if (lines == 3)
		lineConstant = 300;
	if (lines == 4)
		lineConstant = 1200;

	score += lineConstant * (level + 1);

	if ( level < maxLevel)
		level = totalLinesCleared / 15;

}

void GameLogic::setGameOver()
{
	gameOver = true;
	printf("GAME OVER!");
	currentTet->~Tetromino();
	nextTet->~Tetromino();
	thirdTet->~Tetromino();
}

void GameLogic::checkForGameOver()
{
	for (int col = 0; col < mapWidth; ++col)
		if (landed[1][col] != 0)
			setGameOver();
}

void GameLogic::checkForCompletedLines()
{
	int completedLinesCounter = 0;

	for (int row = 0; row < mapHeight; ++row)
	{
		bool lineIsComplete = true;
		for (int col = 0; col < mapWidth; ++col)
			if (landed[row][col] == 0)
				lineIsComplete = false;
		if (lineIsComplete)
		{
			clearLine(row);
			completedLinesCounter++;
		}

	}
	if (completedLinesCounter != 0)
		calculatingScore(completedLinesCounter);

	checkForGameOver();
}

void GameLogic::firmDrop()
{
	while (!firmDropped)
		automaticDrop();

	firmDropped = false;
}

void GameLogic::setAI(AI* ai)
{
	this->ai = ai;
}

// GETTERS

bool GameLogic::getGameOver()
{
	return gameOver;
}

int GameLogic::getLevel()
{
	return level;
}

int GameLogic::getScore()
{
	return score;
}

Tetromino* GameLogic::getCurrentTetromino()
{
	return currentTet;
}

Tetromino* GameLogic::getNextTetromino()
{
	return nextTet;
}

Tetromino* GameLogic::getThirdTetromino()
{
	return thirdTet;
}

int GameLogic::getMapHeight()
{
	return mapHeight;
}

int GameLogic::getMapWidth()
{
	return mapWidth;
}

vector< vector<int> > GameLogic::getLandedMatrix()
{
	return landed;
}

GameLogic::~GameLogic()
{

}