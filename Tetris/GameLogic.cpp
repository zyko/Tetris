#include "GameLogic.h"



using namespace std;


GameLogic::GameLogic()
{
	finishedAIplays				= false;
	geneticAlgorithmComputing	= false;
	singlePlayer				= false;

	totalGamesPlayed = 0;

	// initializing landed array
	vector <int> tmp;
	for (int i = 0; i < mapWidth; ++i)
		tmp.push_back(0);

	for (int col = 0; col < mapHeight; ++col)
		landed.push_back(tmp);

	randomBag = { 0, 1, 2, 3, 4, 5, 6 };

	initializeTetros();
}

// this is called at the very beginning of the game and if game's restarting
void GameLogic::initializeTetros()
{
	/* initialize random seed: */
	srand(time(NULL));

	// due to the possibility to reset the game at any time, randomBag can be empty any time
	if (randomBag.size() <= 0)
		randomBag = { 0, 1, 2, 3, 4, 5, 6 };
	int rndInt = rand() % randomBag.size();
	currentTet = new Tetromino(randomBag[rndInt]);
	randomBag.erase(randomBag.begin() + rndInt);

	// due to the possibility to reset the game at any time, randomBag can be empty any time
	if (randomBag.size() <= 0)
		randomBag = { 0, 1, 2, 3, 4, 5, 6 };
	rndInt = rand() % randomBag.size();
	nextTet = new Tetromino(randomBag[rndInt]);
	randomBag.erase(randomBag.begin() + rndInt);

	// due to the possibility to reset the game at any time, randomBag can be empty any time
	if (randomBag.size() <= 0)
		randomBag = { 0, 1, 2, 3, 4, 5, 6 };
	rndInt = rand() % randomBag.size();
	thirdTet = new Tetromino(randomBag[rndInt]);
	randomBag.erase(randomBag.begin() + rndInt);

}

// drop method for GA
bool GameLogic::dropAI()
{

	currentTet->topLeft[0] = bottomLineTarget;
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (currentTet->getShape()[row][col] != 0)
					landed[row + currentTet->topLeft[0]][col + currentTet->topLeft[1]] = currentTet->getShape()[row][col];

	tetroHasLanded();
	return true;
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

void GameLogic::firmDrop()
{
	while (!firmDropped)
		automaticDrop();

	firmDropped = false;
}

// ---------------------------------


void GameLogic::tetroHasLanded()
{
	firmDropped = true;
	int completedLines = checkForCompletedLines(&landed);
	if (completedLines > 0)
		calculatingScore(completedLines);
	
	if (geneticAlgorithmComputing)
		if (ai->checkForReset(amountOfTetrominosDropped) || checkForGameOver())
			setGameOver();
		else
			spawningNewTetro();

	/* this is important to not spawn a new tetro when generic algorithm triggers a reset */

	if (singlePlayer || finishedAIplays)
	{
		if (checkForGameOver())
			setGameOver();
		else
			spawningNewTetro();
	}
		
}

void GameLogic::spawningNewTetro()
{
	if (!gameOver)
	{
		amountOfTetrominosDropped++;

		delete currentTet;
		currentTet = nextTet;
		nextTet = thirdTet;

		if (!singlePlayer)
			ai->moveTetromino();


		/* initialize random seed: */
		srand(time(NULL));

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
			{	// prevent from moving "out of bounds" / playing area (only for single player necessary)
		if (col + currentTet->topLeft[1] < 0)			// left side
		{
			currentTet->rotate(rotateBack);		// rotating again to 360° 
		}
		else if (col + currentTet->topLeft[1] >= mapWidth)	// right side
		{
			currentTet->rotate(rotateBack);		// rotating again to 360° 
		}
		else if (row + currentTet->topLeft[0] >= mapHeight)	// bottom
		{
			currentTet->rotate(rotateBack);		// rotating again to 360° 
		}
		else if (landed[row + currentTet->topLeft[0]][col + currentTet->topLeft[1]] != 0) // space is already taken
		{
			currentTet->rotate(rotateBack);
		}
			}
}

int GameLogic::checkForCompletedLines(std::vector< std::vector<int> > *landedMatrix)
{
	int completedLinesCounter = 0;

	for (int row = 0; row < mapHeight; ++row)
	{
		bool lineIsComplete = true;
		for (int col = 0; col < mapWidth; ++col)
			if ((*landedMatrix)[row][col] == 0)
				lineIsComplete = false;
		if (lineIsComplete)
		{
			clearLine(row, landedMatrix);
			completedLinesCounter++;
		}
	}

	return completedLinesCounter;

}

void GameLogic::clearLine(int index, std::vector< std::vector<int> > *landedMatrix)
{
	int toClear = index;
	for (toClear; toClear >= invisibleTopLines; --toClear)
		for (int col = 0; col < mapWidth; ++col)
			(*landedMatrix)[toClear][col] = (*landedMatrix)[toClear - 1][col];
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

bool GameLogic::checkForGameOver()
{
	for (int col = 0; col < mapWidth; ++col)
		if (landed[1][col] != 0)
			return true;
	
	return false;
}

void GameLogic::setGameOver()
{
	gameOver = true;
	currentTet->~Tetromino();
	nextTet->~Tetromino();
	thirdTet->~Tetromino();

	if (!singlePlayer && !ai->isGAterminating())
		resetGame();
}


// this is for Genetic Algorithm:


void GameLogic::resetGame()
{
	totalGamesPlayed++;

	// resetting landed array
	for (int row = 0; row < mapHeight; ++row)
		for (int col = 0; col < mapWidth; ++col)
			landed[row][col] = 0;




	initializeTetros();
	
	ai->gameHasFinished(&totalLinesCleared, &totalGamesPlayed);




	gameOver = false;
	amountOfTetrominosDropped = 0;
	totalLinesCleared = 0;
	level = 0;
	score = 0;
	
}

// --------------------


// SETTER

void GameLogic::setGamePlay(int i)
{
	if (i == 1)
		singlePlayer = true;
	else if (i == 2)
		finishedAIplays = true;
	else if (i == 3)
		geneticAlgorithmComputing = true;
	
}

void GameLogic::setAI(AI* ai)
{
	this->ai = ai;
}

// GETTERS

bool GameLogic::getFinishedAIPlays()
{
	return finishedAIplays;
}

bool GameLogic::getGeneticAlgorithmComputing()
{
	return geneticAlgorithmComputing;
}

int GameLogic::getTotalGamesPlayed()
{
	return totalGamesPlayed;
}

int GameLogic::getAmountOfTetrominosDropped()
{
	return amountOfTetrominosDropped;
}

bool GameLogic::getSinglePlayer()
{
	return singlePlayer;
}

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
	delete currentTet;
	delete nextTet;
	delete thirdTet;
}