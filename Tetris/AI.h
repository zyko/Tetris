#pragma once

#include "Common.h"
#include "Tetromino.h"
#include "GameLogic.h"
#include <cmath>
#include <iostream>	// this is used for saving to data
#include <fstream>	// this is used for saving to data
#include <time.h>	// this is used for random function
#include <random>	// this is used for random function

class AI
{

private:

	/* THIS IS FOR THE GENETIC ALGORITHM */

	float aggregateHeightParameter	= -0.51f;
	float completeLinesParameter	=  0.76f;
	float holesParameter			= -0.35f;
	float bumpinessParameter		= -0.18f;


	const int totalGamesToBePlayedForOneIndividual			= 10;
	const int totalTetrominosToBeDroppedForOneIndividual	= 100; // should be 500 or sth.
	const int maxPopulation									= 1000; // means individuals is filled, until individuals.size() == maxPopulation
	const int amountOfRandomIndividualsForReproducing		= maxPopulation / 10; // chose 10 % random for producing offspring
	const int amountOfBestChosen							= 2; // choosing the best 2 out of the random chosen individuals
	const int maxOffspringsProduced							= (maxPopulation / 10 ) * 3; // 30 % of maxPopulation will be new offsprings

	int currentIndividualIndex = 0; // just use population.size() instead?


	struct individual
	{
		/* parameters: */
		float aggregateHeightParameter;
		float completeLinesParameter;
		float holesParameter;
		float bumpinessParameter;

		int completedLines;
	};

	std::vector <individual> population;
	std::vector <individual> offsprings;

	

	void saveLinesClearedToIndividual(int totalLinesCleared);

	void saveFinalParametersToFile();


	void setParameters(float aggregateHeightParameter, float completeLinesParameter, float holesParameter, float bumpinessParameter);


	void initializePopulation();
	void chooseNextIndividual();
	float mutate();
	void chooseRandomIndividuals();
	void chooseBestParents();
	void produceOffspring(int parentAindex, int parentBindex);
	void replaceWeakestPopulation();
	void deleteWeakestIndividualFromPopulation();
	bool isAlgorithmTerminating();

	void startGeneticLoop();

public:
	void gameHasFinished(int* totalLinesCleared, int* totalGamesPlayed);
	bool checkForReset(int amountOfTetrominosDropped);
	
	
	int getCurrentIndividualIndex();

	int getTotalGamesToBePlayedForOneIndividual();
	int getTotalTetrominosToBeDroppedForOneIndividual();
	int getMaxPopulation();
	
	
	/* --------------------------------- */

private:

	// this struct saves the information for the next tetromino how and where to be dropped.
	// it is computed by the AI
	struct computationStruct
	{
		float result;
		int position; // col // this is actually topleft Y position, so topLeft[1], or col
		int rotation;
		int bottomLineTarget; // row // this is actually topleft X position, so topLeft[0], or row
	};

	std::vector <computationStruct> computedResults;


	// variables:

	bool computationDone = false;
	bool hasLanded = false;
	bool reachedTheVeryRight = false;

	// array used for computations
	std::vector< std::vector<int> > cpt;

	


	Tetromino* currentTet;
	Tetromino* nextTet;
	GameLogic* gameLogic;


	// functions:

	bool tryToSpawn(Tetromino* tet, int position);

	void tryCurrentTet(bool initialCall);

	float tryNextTet(std::vector< std::vector<int> > droppedCurrentMatrix);

	void dropTetromino(Tetromino* tet);


	/*
	void moveTetrominoToVeryLeft(Tetromino* tet);
	void moveTetrominoOneTileToTheRight(Tetromino* tet);
	*/

	float computation();
	int computeAggregateHeight();
	int computeCompleteLines();
	int computeHoles();
	int computeBumpiness();

public:

	// AI's very own landed array. always adding one tetromino to it before gamelogic.cpp does
	std::vector< std::vector<int> > landedAI;

	void initializeAI(bool veryFirstMethodCall);
	void resetLandedAI();
	void moveTetromino();
	
	void makeDecision(bool initialCall);

	
	std::vector<float> getParameters();

	AI(GameLogic* gl);
	~AI();
};