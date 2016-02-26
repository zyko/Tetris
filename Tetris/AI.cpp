#include "AI.h"

using namespace std;


AI::AI(GameLogic* gl)
{
	gameLogic = gl;
	currentIndividualIndex = 0; // just use population.size() instead?
	amountOfGAloops = 0;
}

// this is called at the very beginning of the game or when the game resets
void AI::initializeAI(bool veryFirstMethodCall)
{
	landedAI = gameLogic->getLandedMatrix();

	if (veryFirstMethodCall)
	{
		if (gameLogic->getGeneticAlgorithmComputing())
		{
			initializePopulation();
			aggregateHeightParameter = population[0].aggregateHeightParameter;
			completeLinesParameter = population[0].completeLinesParameter;
			holesParameter = population[0].holesParameter;
			bumpinessParameter = population[0].bumpinessParameter;
		}
		else if (gameLogic->getFinishedAIPlays())
		{

			float aggregateHeightParameterFromFile;
			float completeLinesParameterFromFile;
			float holesParameterFromFile;
			float bumpinessParameterFromFile;


			ifstream source;									// build a read-Stream

			source.open("finalParameters.txt", ios_base::in);  // open data

			if (!source)
				printf("cannot open finalParameter.txt");

			for (std::string line; std::getline(source, line);)   
			{
				std::istringstream in(line);      

				std::string type;
				in >> type;                 

				if (type == "aggregateHeightParameter:") 
				{
					in >> aggregateHeightParameterFromFile; 
					printf("loaded from file: %f \n", aggregateHeightParameterFromFile);
				}
				if (type == "completeLinesParameter:")  
				{
					in >> completeLinesParameterFromFile;    
					printf("loaded from file: %f \n", completeLinesParameterFromFile);
				}
				else if (type == "holesParameter:")
				{
					in >> holesParameterFromFile;      
					printf("loaded from file: %f \n", holesParameterFromFile);
				}
				else if (type == "bumpinessParameter:")
				{
					in >> bumpinessParameterFromFile; 
					printf("loaded from file: %f \n", bumpinessParameterFromFile);
				}
			}
			

			aggregateHeightParameter	= aggregateHeightParameterFromFile;
			completeLinesParameter		= completeLinesParameterFromFile;
			holesParameter				= holesParameterFromFile;
			bumpinessParameter			= bumpinessParameterFromFile;
		}

	}


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
	amountOfGAloops++;
	while (offsprings.size() < maxOffspringsProduced)
		chooseRandomIndividuals();

	// produceOffspring is called by upper method

	replaceWeakestPopulation();
}

// actually not only choosing random individuals, but also picking the best and triggering produceOffspring()
void AI::chooseRandomIndividuals()
{
	/* choosing random individuals */

	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector <int> chosenIndividuals;

	/* creating new vector with shuffled integers from 0 - maxPopulation */
	for (int i = 0; i < population.size(); ++i)
		chosenIndividuals.push_back(i);

	std::shuffle(chosenIndividuals.begin(), chosenIndividuals.end(), mt);


	/* choose the 2 best out of the "amountOfRandomIndividualsForReproducing" random individuals */

	int bestIndividualIndex = 0;
	int bestIndividualLines = 0;

	int secondBestIndividualIndex = 0;
	int secondBestIndividualLines = 0;


	for (int i = 0; i < amountOfRandomIndividualsForReproducing; ++i)
	{
		if (population[chosenIndividuals[i]].completedLines > bestIndividualLines)
		{

			/* set former best to current 2nd best */
			secondBestIndividualLines = bestIndividualLines;
			secondBestIndividualIndex = bestIndividualIndex;

			/* set best to current checked*/
			bestIndividualLines = population[chosenIndividuals[i]].completedLines;
			bestIndividualIndex = chosenIndividuals[i];


		}	// this is for the case, that either there are 2 best individuals with same completedLines, or 1st in array is already best
		else if (population[chosenIndividuals[i]].completedLines > secondBestIndividualLines)
		{
			secondBestIndividualLines = population[chosenIndividuals[i]].completedLines;
			secondBestIndividualIndex = chosenIndividuals[i];
		}
	}


	/* produce new offspring of the two parents picked */

	produceOffspring(bestIndividualIndex, secondBestIndividualIndex);
}

void AI::produceOffspring(int parentAindex, int parentBindex)
{

	float newAggregateHeightParameter	= ((population[parentAindex].aggregateHeightParameter	+ population[parentBindex].aggregateHeightParameter ) / 2);
	float newCompleteLinesParameter		= ((population[parentAindex].completeLinesParameter		+ population[parentBindex].completeLinesParameter	) / 2);
	float newHolesParameter				= ((population[parentAindex].holesParameter				+ population[parentBindex].holesParameter			) / 2);
	float newBumpinessParameter			= ((population[parentAindex].bumpinessParameter			+ population[parentBindex].bumpinessParameter		) / 2);


	offsprings.push_back({ newAggregateHeightParameter, newCompleteLinesParameter, newHolesParameter, newBumpinessParameter });

	mutate();
}

void AI::mutate()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(0.0, 0.2);
	std::uniform_real_distribution<float> dist2(0, 4);
	std::uniform_real_distribution<float> dist3(0, 20);

	int mutationChance = dist3(mt);
	int parameterToMutate = dist2(mt);
	float mutationValue = (round(dist1(mt) * 1000.f)) / 1000.f;

	if (mutationChance == 1) // 5 % chance to mutate ( 1 I chose randomly, could take any number between 0 and 20 )
	{
		/* mutate either one of the parameters */

		if (parameterToMutate == 1)
			offsprings.back().aggregateHeightParameter += mutationValue;
		if (parameterToMutate == 2)
			offsprings.back().completeLinesParameter += mutationValue;
		if (parameterToMutate == 3)
			offsprings.back().holesParameter += mutationValue;
		if (parameterToMutate == 4)
			offsprings.back().bumpinessParameter += mutationValue;
	}
}

void AI::replaceWeakestPopulation()
{

	/* delete the weakest individuals from population */

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

void AI::initializePopulation()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(0.f, 1.f);
	std::uniform_real_distribution<float> dist2(0.f, 1.f);
	std::uniform_real_distribution<float> dist3(0.f, 1.f);
	std::uniform_real_distribution<float> dist4(0.f, 1.f);


	for (int i = 0; i < maxPopulation; ++i)
		population.push_back({ -dist1(mt), dist2(mt), -dist3(mt), -dist4(mt), 0 });
}

void AI::chooseNextIndividual()
{
	if (currentIndividualIndex < maxPopulation-1)
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
		currentIndividualIndex = 0;
	}
}

void AI::setParameters(float aggregateHeightParameter, float completeLinesParameter, float holesParameter, float bumpinessParameter)
{
	this->aggregateHeightParameter	= aggregateHeightParameter;
	this->completeLinesParameter	= completeLinesParameter;
	this->holesParameter			= holesParameter;
	this->bumpinessParameter		= bumpinessParameter;
}

bool AI::isGAterminating()
{
	if (amountOfGAloops >= maxGAloops)
	{
		saveFinalParametersToFile();
		return true;
	}

	return false;
}

void AI::saveFinalParametersToFile()
{
	float averageAggregateHeightParameter = 0;
	float averageCompleteLinesParameter = 0;
	float averageHolesParameter = 0;
	float averageBumpinessParameter = 0;

	for (int i = 0; i < population.size(); ++i)
	{
		averageAggregateHeightParameter	+= population[i].aggregateHeightParameter;
		averageCompleteLinesParameter	+= population[i].completeLinesParameter;
		averageHolesParameter			+= population[i].holesParameter;
		averageBumpinessParameter		+= population[i].bumpinessParameter;
	}

	averageAggregateHeightParameter /= population.size();
	averageCompleteLinesParameter	/= population.size();
	averageHolesParameter			/= population.size();
	averageBumpinessParameter		/= population.size();


	ofstream myfile("finalParameters.txt");
	if (myfile.is_open())
	{
		myfile << "aggregateHeightParameter: " << averageAggregateHeightParameter << endl;
		myfile << "completeLinesParameter: " << averageCompleteLinesParameter << endl;
		myfile << "holesParameter: " << averageHolesParameter << endl;
		myfile << "bumpinessParameter: " << averageBumpinessParameter << endl;

		myfile.close();
	}
	else cout << "Unable to open file";

	printf("final parameters have been saved to file");
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
	computation += (completeLinesParameter *	computeCompleteLines());
	computation += (holesParameter *			computeHoles());
	computation += (bumpinessParameter *		computeBumpiness());

	return computation;

}

int AI::computeAggregateHeight()
{
	int tmpCounter = 0;
	int aggregateHeight = 0;


	for (int col = 0; col < gameLogic->getMapWidth(); ++col)
	{
		tmpCounter = 0;
		for (int row = gameLogic->getMapHeight()-1; row > 0; --row)
		{
			tmpCounter++;

			if (cpt[row][col] != 0)
			{
				aggregateHeight += tmpCounter;
				tmpCounter = 0;
			}
			
		}
	}

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
	int currentRowHeight = 0;

	for (int col = 0; col < gameLogic->getMapWidth(); ++col)
	{
		lastRowHeight = currentRowHeight;
		currentRowHeight = 0;

		for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		{
			if (cpt[row][col] != 0)
			{
				currentRowHeight = gameLogic->getMapHeight() - row;
				break;
			}
		}

		if (col == 0)
			lastRowHeight = currentRowHeight;
	
		bumpiness += abs(currentRowHeight - lastRowHeight);
	}

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