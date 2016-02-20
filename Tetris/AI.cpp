#include "AI.h"

using namespace std;


AI::AI(GameLogic* gl)
{
	gameLogic = gl;

	//landedAI = gl->getLandedMatrix();

	if (gameLogic->getGeneticAlgorithmComputing())
		initializePopulation();
}

// this is called at the very beginning of the game or when the game resets
void AI::initializeAI(bool veryFirstMethodCall)
{
	landedAI = gameLogic->getLandedMatrix();

	makeDecision(true);
	moveTetromino();

	if (veryFirstMethodCall)
		makeDecision(false);
}

void AI::makeDecision(bool initialCall)
{
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

	gameLogic->checkForCompletedLines(&landedAI);



	/*
	// DEBUGGING
	printf("bottom LineTarget: %d \n", computedResults[0].bottomLineTarget);
	printf("position: %d \n", computedResults[0].position);
	// turn this off for a second
	printf("---- BEST COMPUTATION VALUE ---- \n");
	printf("maxVal: %f \n", computedResults[0].result);
	printf("position: %d \n", computedResults[0].position);
	printf("rotation: %d \n", computedResults[0].rotation);
	*/
}

void AI::tryCurrentTet(bool initialCall)
{
	// creating a copy of gameLogic's real Tetrominos
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
				currentTet->topLeft[1] = col - 2;

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
				// in this landed matrix, the current Tetromino we're trying should be landed
				//std::vector< std::vector<int> > droppedCurrentMatrix = cpt;
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

	delete currentTet;
	delete nextTet;
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
				nextTet->topLeft[1] = col - 2;

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

void AI::moveTetromino()
{
	// set real current Tetromino to desired position
	gameLogic->bottomLineTarget = computedResults[0].bottomLineTarget;
	gameLogic->getCurrentTetromino()->rotate(computedResults[0].rotation);
	gameLogic->getCurrentTetromino()->topLeft[1] = computedResults[0].position;
	
}

// 1st step:
// this is used for trying to spawn the tetromino on x-axis

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

// 2nd step:
// this is used for the tetromino to be dropped on y-axis

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


/* methods for Genetic Algorithm (GA) */

#pragma region this is the genetic loop

void AI::startGeneticLoop()
{

	while (offsprings.size() < maxOffspringsProduced)
		chooseRandomIndividuals();

	// produceOffspring is called by upper method

	replaceWeakestPopulation();

	/*	shit ain't workin
		1st: check for dropped tetrominos
		2nd: check for played games
		3rd: */


	/*	1st: for each individual in population play 10 games á 500 tetrominos or until game over
		2nd: once finished with this, do that stuff with offsprings
		3rd: return to 1.


		todo: check for termination anywhere? :O
	*/
}

void AI::chooseRandomIndividuals()
{
	/* choosing random individuals */

	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector <int> debug;

	for (int i = 0; i < amountOfRandomIndividualsForReproducing; ++i)
		debug.push_back(i);

	std::shuffle(debug.begin(), debug.end(), mt);



	/* choose the 2 best out of the 100 random individuals */

	int bestIndividualIndex = 0;
	int bestIndividualLines = 0;

	int secondBestIndividualIndex = 0;
	int secondBestIndividualLines = 0;


	for (int i = 0; i < debug.size(); ++i)
	{
		if (population[debug[i]].completedLines > bestIndividualLines)
		{
			/* set former best to current 2nd best */
			secondBestIndividualLines = bestIndividualLines;
			secondBestIndividualIndex = bestIndividualIndex;

			/* set best to current checked*/
			bestIndividualLines = population[debug[i]].completedLines;
			bestIndividualIndex = i;
		}
	}

	/* produce new offspring of the two parents picked */

	produceOffspring(bestIndividualIndex, secondBestIndividualIndex);


	/* DEBUG ONLY

	printf("random numbers are:");
	for (int i = 0; i < amountOfRandomIndividualsForReproducing; ++i)
	printf("%d \n", debug[i]);
	*/

}

// yet, implementation has to be finished
void AI::produceOffspring(int parentAindex, int parentBindex)
{
	population[parentAindex];
	population[parentBindex];
}

void AI::replaceWeakestPopulation()
{

	/* delete the 100 weakest individuals from population */

	while (population.size() > (maxPopulation - maxOffspringsProduced))
	{
		deleteWeakestIndividualFromPopulation();
	}


	/* replace them with produced offspring */

	while (population.size() < maxPopulation) // this should actually be redundant, because population.size() + offsprings.size() should be equal to maxPopulation
	{
		for (individual iv : offsprings)
		{
			population.push_back(iv);
		}
	}

	offsprings.clear();
}

void AI::deleteWeakestIndividualFromPopulation()
{
	int weakestIndividualIndex = 0;
	int weakestIndividualLines = 0;

	for (int i = 0; i < population.size(); ++i)
	{
		if (population[i].completedLines < weakestIndividualLines)
		{
			weakestIndividualIndex = i;
		}
	}

	population.erase(population.begin() + weakestIndividualIndex);
}

#pragma endregion


// checking after each tetromino has landed
// returning true means the game is being reset and gameHasFinished() in AI.cpp is called
bool AI::checkForReset(int amountOfTetrominosDropped)
{
	if (amountOfTetrominosDropped >= totalTetrominosToBeDroppedForOneIndividual)
		return true;
	
	return false;
}

// this is called from gamelogic.cpp once a game has finished (gameover or specific amount of dropped tetrominos has been reached
void AI::gameHasFinished(int* totalLinesCleared, int* totalGamesPlayed)
{
	/* resetting AI for next game */
	resetLandedAI();
	initializeAI(false);


	if (*totalGamesPlayed >= totalGamesToBePlayedForOneIndividual)
	{
		saveLinesClearedToIndividual(*totalLinesCleared);

		*totalGamesPlayed = 0;
		*totalLinesCleared = 0;

		chooseNextIndividual();
	}
}

void AI::resetLandedAI()
{
	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			landedAI[row][col] = 0;
}

void AI::saveLinesClearedToIndividual(int totalLinesCleared)
{
	population[currentIndividualIndex].completedLines = totalLinesCleared;
}

// yet, there is still some implementation missing
bool AI::isAlgorithmTerminating()
{
	// uhm, got u fucking clue?

	return false;
}

void AI::initializePopulation()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(0.f, 1.f);
	std::uniform_real_distribution<float> dist2(0.f, 1.f);
	std::uniform_real_distribution<float> dist3(0.f, 1.f);
	std::uniform_real_distribution<float> dist4(0.f, 1.f);


	for (int i = 0; i < maxPopulation; ++i)
	{
		population.push_back({ -dist1(mt), dist2(mt), -dist3(mt), -dist4(mt), 0 });
	}


	/* debug only */
	//float a = dist1(mt);
	//float aa = dist1(mt);
	//float b = dist2(mt);
	//float c = dist3(mt);
	//float d = dist4(mt);

	//printf("1st rnd number: %f \n", a);
	//printf("2nd rnd number: %f \n", b);
	//printf("3rd rnd number: %f \n", c);
	//printf("4th rnd number: %f \n", d);
}

void AI::chooseNextIndividual()
{
	if (currentIndividualIndex < maxPopulation)
	{
		currentIndividualIndex++;

		setParameters(	population[currentIndividualIndex].aggregateHeightParameter,
						population[currentIndividualIndex].completeLinesParameter,
						population[currentIndividualIndex].holesParameter,
						population[currentIndividualIndex].bumpinessParameter);
	}
	else
	{
		startGeneticLoop();
	}
}

// this is not yet called
float AI::mutate()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(0.0, 0.2);
	std::uniform_real_distribution<float> dist2(0, 4);
	std::uniform_real_distribution<float> dist3(0, 20);

	int mutationChance		= dist3(mt);
	int parameterToMutate	= dist2(mt);
	float mutationValue		= (round (dist1(mt) * 1000.f) ) / 1000.f;

	if (mutationChance == 13) // 5 % chance to mutate ( 13 is random, could take any number between 0 and 20 )
	{
		//debug:
		printf("mutation has been triggered");
		
		/* mutate either one of the parameters */
		
		if (parameterToMutate == 1)
			population.back().aggregateHeightParameter += mutationValue;
		if (parameterToMutate == 2)
			population.back().completeLinesParameter += mutationValue;
		if (parameterToMutate == 3)
			population.back().holesParameter += mutationValue;
		if (parameterToMutate == 4)
			population.back().bumpinessParameter += mutationValue;
	}

	return 1.f;
}

void AI::setParameters(float aggregateHeightParameter, float completeLinesParameter, float holesParameter, float bumpinessParameter)
{
	this->aggregateHeightParameter	= aggregateHeightParameter;
	this->completeLinesParameter	= completeLinesParameter;
	this->holesParameter			= holesParameter;
	this->bumpinessParameter		= bumpinessParameter;
}

// this is not yet called
void AI::saveFinalParametersToFile()
{
	ofstream myfile("finalParameters.txt");
	if (myfile.is_open())
	{
		myfile << "The aggregate height parameter is: " << aggregateHeightParameter << endl;
		myfile << "The complete lines parameter is: " << completeLinesParameter << endl;
		myfile << "The holes parameter is: " << holesParameter << endl;
		myfile << "The bumpiness parameter is: " << bumpinessParameter << endl;

		myfile.close();
	}
	else cout << "Unable to open file";
}


/* simple getters */

int AI::getTotalGamesToBePlayedForOneIndividual()
{
	return totalGamesToBePlayedForOneIndividual;
}
int AI::getTotalTetrominosToBeDroppedForOneIndividual()
{
	return totalTetrominosToBeDroppedForOneIndividual;
}
int AI::getMaxPopulation()
{
	return maxPopulation;
}

int AI::getCurrentIndividualIndex()
{
	return currentIndividualIndex;
}

std::vector<float> AI::getParameters()
{
	std::vector<float> parameters = { aggregateHeightParameter, completeLinesParameter, holesParameter, bumpinessParameter };
	return parameters;
}

/* -------------------------------------- */

// computation

float AI::computation()
{

	float computation = 0;

	computation += (aggregateHeightParameter *	computeAggregateHeight());
	computation += (completeLinesParameter *		computeCompleteLines());
	computation += (holesParameter *		computeHoles());
	computation += (bumpinessParameter *	computeBumpiness());

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

//-----------------


// getters
// actually for multithreading

// destructor

AI::~AI()
{
	delete currentTet;
	delete nextTet;
	delete gameLogic;
}