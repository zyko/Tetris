#pragma once

#include "Common.h"
#include "Tetromino.h"
#include "GameLogic.h"
#include <cmath>

class AI
{

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

	int debugTetroCounter = 0;

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

	void moveTetromino();
	void makeDecision(bool initialCall);
	AI(GameLogic* gl);
	~AI();
};