#include "AI.h"

using namespace std;


AI::AI(GameLogic* gl)
{
	gameLogic = gl;

	landedAI = gl->getLandedMatrix();
}

void AI::makeDecision(bool initialCall)
{
	computationDone = false;
	tryCurrentTet(initialCall);


	/* once the computation is done,
	* the computed tetromino is already added to the AI's very own landed array,
	* so next computation can be done, although actual current Tetromino in gamelogic.cpp hasn't even landed yet.
	*/
	if (initialCall)
	{
		Tetromino *tmp = new Tetromino(gameLogic->getCurrentTetromino()->getType());
		tmp->rotate(computedResults[0].rotation);
		tmp->topLeft[1] = computedResults[0].position;
		tmp->topLeft[0] = computedResults[0].bottomLineTarget;

		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (tmp->getShape()[row][col] != 0)
					landedAI[row + tmp->topLeft[0]][col + tmp->topLeft[1]] = tmp->getShape()[row][col];

		delete tmp;
	}
	else if (!initialCall)
	{
		Tetromino *tmp = new Tetromino(gameLogic->getNextTetromino()->getType());
		tmp->rotate(computedResults[0].rotation);
		tmp->topLeft[1] = computedResults[0].position;
		tmp->topLeft[0] = computedResults[0].bottomLineTarget;

		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (tmp->getShape()[row][col] != 0)
					landedAI[row + tmp->topLeft[0]][col + tmp->topLeft[1]] = tmp->getShape()[row][col];

		delete tmp;
	}

	printf("bottom LineTarget: %d \n", computedResults[0].bottomLineTarget);
	printf("position: %d \n", computedResults[0].position);
	printf("computed tetrominos in total: %d \n", ++debugTetroCounter),


	// this is for multithreading
	computationDone = true;

	/* turn this off for a second
	printf("---- BEST COMPUTATION VALUE ---- \n");
	printf("maxVal: %f \n", computedResults[0].result);
	printf("position: %d \n", computedResults[0].position);
	printf("rotation: %d \n", computedResults[0].rotation);
	*/
}



void AI::moveTetromino()
{
	// set real current Tetromino to desired position
	gameLogic->bottomLineTarget = computedResults[0].bottomLineTarget;
	gameLogic->getCurrentTetromino()->rotate(computedResults[0].rotation);
	gameLogic->getCurrentTetromino()->topLeft[1] = computedResults[0].position;
	
}

// returns highest computation value for the 2nd tetromino
float AI::tryNextTet(std::vector< std::vector<int> > droppedCurrentMatrix)
{

	float maxCompVal = -std::numeric_limits<float>::max();
	int maxRotations = 4;


	int counter = 0; // debug


	if (nextTet->getType() == 0 || nextTet->getType() == 4 || nextTet->getType() == 5) // Tetros 'I', 'S' and 'Z' have only 2 rotations
		maxRotations = 2;
	if (nextTet->getType() == 1)	// Tetro 'O' has only 1 rotation
		maxRotations = 1;

	for (int rotations = 0; rotations < maxRotations; ++rotations)
	{
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
		{
			if (tryToSpawn(nextTet, col - 2)) // tries to respawn on different y / col locations. from very left to very right
			{
				cpt = droppedCurrentMatrix;

				nextTet->topLeft[0] = 0;
				nextTet->topLeft[1] = col-2;

				// todo: should actually be called
				/*while (!hasLanded)
				{
					dropTetromino(nextTet);
				}*/

				float cmp = computation();

				if (cmp > maxCompVal)
				{
					maxCompVal = cmp;
				}
			}
		}
		nextTet->rotate(1);
	}
	return maxCompVal;
}

void AI::tryCurrentTet(bool initialCall)
{
	if (initialCall)
	{
		currentTet = new Tetromino(gameLogic->getCurrentTetromino()->getType());
		nextTet = new Tetromino(gameLogic->getNextTetromino()->getType());
	}
	else if (!initialCall)
	{
		currentTet = new Tetromino(gameLogic->getNextTetromino()->getType());
		nextTet = new Tetromino(gameLogic->getThirdTetromino()->getType());
	}
	

	int counter = 0; // debug
	computedResults.resize(1);

	float maxCompVal = -std::numeric_limits<float>::max();

	int maxRotations = 4;
	if (currentTet->getType() == 0 || currentTet->getType() == 4 || currentTet->getType() == 5) // Tetros 'I', 'S' and 'Z' have only 2 rotations
		maxRotations = 2;
	if (currentTet->getType() == 1)	// Tetro 'O' has only 1 rotation
		maxRotations = 1;

	for (int rotations = 0; rotations < maxRotations; ++rotations)
	{
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
		{
			if (tryToSpawn(currentTet, col - 2)) // tries to respawn on different y / col locations. from very left to very right
			{
				cpt = landedAI;


				currentTet->topLeft[0] = 0;
				currentTet->topLeft[1] = col-2;

				while (!hasLanded)
				{
					dropTetromino(currentTet);
				}
					

				hasLanded = false;

				// debugging:
				/* turn this off for a second
				
				printf("--------------------\n");
				printf("computation no: %d \n", counter++);
				printf("rotation: %d \n", rotations);
				printf("currentTet->topLeft[1]: %d \n", currentTet->topLeft[1]);
				*/

				// try next Tetromino as well with current landed matrix
				std::vector< std::vector<int> > droppedCurrentMatrix = cpt;
				float cmp = computation();// +tryNextTet(droppedCurrentMatrix);


				if (cmp > maxCompVal)
				{
					maxCompVal = cmp;
					computedResults[0] = { cmp, currentTet->topLeft[1], rotations, currentTet->topLeft[0] };
				}	
			}
		}
		currentTet->rotate(1);
	}

	printf("delete was called");
	delete currentTet;
	delete nextTet;
}

bool AI::tryToSpawn(Tetromino* tet, int position)
{
	bool ableToSpawn = true;

		// check for map end
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (tet->getShape()[row][col] != 0)
					if (col + position < 0 || col + position >= gameLogic->getMapWidth())
						return false;

	return ableToSpawn;
}

// moving Tetrominos

void AI::dropTetromino(Tetromino* tet)
{
	// checking for collisions
	vector <int> potentialTopLeft = { tet->topLeft[0] + 1, tet->topLeft[1] };
	bool landing = false;

	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			if (tet->getShape()[row][col] != 0)
				if (row + potentialTopLeft[0] >= gameLogic->getMapHeight())					// either the array is empty and it hits the ground
					landing = true;
				else if (cpt[row + potentialTopLeft[0]][col + potentialTopLeft[1]] != 0)	// or it hits previous tetros within the landing array
					landing = true;

	// adding current tetromino to landed array
	if (landing)
	{
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (tet->getShape()[row][col] != 0)
					cpt[row + tet->topLeft[0]][col + tet->topLeft[1]] = tet->getShape()[row][col];
		hasLanded = true;
	}
	else
		tet->topLeft[0]++;
}

// computation

float AI::computation()
{
	float heightConstant =		-0.51;
	float linesConstant =		 0.76;
	float holesConstant =		-0.35;
	float bumpinessConstant =	-0.18;
	float computation = 0;

	computation += (heightConstant *	computeAggregateHeight());
	computation += (linesConstant *		computeCompleteLines());
	computation += (holesConstant *		computeHoles());
	computation += (bumpinessConstant *	computeBumpiness());

	return computation;

}

int AI::computeAggregateHeight()
{
	int aggregateHeight = 0;
	for (int col = 0; col < gameLogic->getMapWidth(); ++col)
		for (int row = 0; row < gameLogic->getMapHeight(); ++row)
			if (cpt[row][col] != 0)
			{
				aggregateHeight += gameLogic->getMapHeight() - row;
				break;
			}
	

	//printf("aggregateHeight: %d \n", aggregateHeight);
	return aggregateHeight;
}

int AI::computeCompleteLines()
{
	int completeLines = 0;
	
	int completedLinesCounter = 0;
	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
	{
		bool lineIsComplete = true;
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			if (cpt[row][col] == 0)
				lineIsComplete = false;
		if (lineIsComplete)
			completeLines++;
	}

	//printf("complete lines: %d \n", completeLines);
	return completeLines;
}

int AI::computeHoles()
{
	int holes=0;

	for (int col = 0; col < gameLogic->getMapWidth(); ++col)
		for (int row = 0; row < gameLogic->getMapHeight(); ++row)
			if (cpt[row][col] != 0)
				if (row < gameLogic->getMapHeight()-1 && (cpt[row+1][col] == 0))
					holes++;

	//printf("holes: %d \n", holes);
	return holes;
}

int AI::computeBumpiness()
{
	int bumpiness = 0;

	int lastRowHeight = 0;
	int currentRowHeight;

	for (int col = 0; col < gameLogic->getMapWidth(); ++col)
		for (int row = 0; row < gameLogic->getMapHeight(); ++row)
			if (cpt[row][col] != 0 || (row == gameLogic->getMapHeight() - 1 && cpt[row][col] == 0))
			{							// reaches ground
				if (lastRowHeight != 0)
				{
					if (row == gameLogic->getMapHeight() - 1)
						currentRowHeight = 0;
					else
						currentRowHeight = gameLogic->getMapHeight() - row;

					bumpiness += abs(currentRowHeight - lastRowHeight);

					lastRowHeight = currentRowHeight;

				}
				else
					lastRowHeight = gameLogic->getMapHeight() - row;

				break;
			}


	//printf("bumpiness: %d \n", bumpiness);
	return bumpiness;

}

// getters
// actually for multithreading
bool AI::isComputationDone()
{
	return computationDone;
}

// destructor

AI::~AI()
{
	delete currentTet;
	delete nextTet;
	delete gameLogic;
}