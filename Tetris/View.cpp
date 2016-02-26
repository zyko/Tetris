#include "View.h"

View::View(sf::RenderWindow* window)
{
	this->window = window;
	gamePlayChosen = false;

	if (!backgroundTxt.loadFromFile("Assets/background_630x860.png"))
		printf("error while loading background texture \n");
	backgroundSprt.setTexture(backgroundTxt);

	if (!gameOverTxt.loadFromFile("Assets/gameOver_1260x860.png"))
		printf("error while loading gameOver.png \n");
	gameOverSprt.setTexture(gameOverTxt);

	if (!font.loadFromFile("Fonts/courbd.ttf")) // this is courier new bold
		printf("error while loading font \n");

	if (!buttonTexture.loadFromFile("Assets/button2.png"))
		printf("error while loading button texture");


	singlePlayerButtonRect.setSize(sf::Vector2f(buttonTexture.getSize()));
	singlePlayerButtonRect.setTexture(&buttonTexture);
	singlePlayerButtonRect.setPosition((window->getSize().x / 2) - (singlePlayerButtonRect.getSize().x / 2), 200);
	

	finishedAIButtonRect.setSize(sf::Vector2f(buttonTexture.getSize()));
	finishedAIButtonRect.setTexture(&buttonTexture);
	finishedAIButtonRect.setPosition((window->getSize().x / 2) - (finishedAIButtonRect.getSize().x / 2), 400);

	geneticAlgorithmButtonRect.setSize(sf::Vector2f(buttonTexture.getSize()));
	geneticAlgorithmButtonRect.setTexture(&buttonTexture);
	geneticAlgorithmButtonRect.setPosition((window->getSize().x / 2) - (geneticAlgorithmButtonRect.getSize().x / 2), 600);

	singlePlayertxt.setString("single player");
	singlePlayertxt.setFont(font);
	singlePlayertxt.setCharacterSize(40);
	singlePlayertxt.setColor(sf::Color::Black);
	singlePlayertxt.setPosition(sf::Vector2f(singlePlayerButtonRect.getPosition().x + 60, singlePlayerButtonRect.getPosition().y + 50));

	finishedAItxt.setString("finished AI");
	finishedAItxt.setFont(font);
	finishedAItxt.setCharacterSize(40);
	finishedAItxt.setColor(sf::Color::Black);
	finishedAItxt.setPosition(sf::Vector2f(finishedAIButtonRect.getPosition().x + 90, finishedAIButtonRect.getPosition().y + 50));

	geneticAlgorithmtxt.setString("genetic algorithm");
	geneticAlgorithmtxt.setFont(font);
	geneticAlgorithmtxt.setCharacterSize(40);
	geneticAlgorithmtxt.setColor(sf::Color::Black);
	geneticAlgorithmtxt.setPosition(sf::Vector2f(geneticAlgorithmButtonRect.getPosition().x + 20, geneticAlgorithmButtonRect.getPosition().y + 50));


	
	singlePlayerInfotxt.setFont(font);
	singlePlayerInfotxt.setCharacterSize(20);
	singlePlayerInfotxt.setColor(sf::Color::Cyan);
	singlePlayerInfotxt.setPosition(sf::Vector2f(640.f, 100.f));

	parameterAItxt.setFont(font);
	parameterAItxt.setCharacterSize(20);
	parameterAItxt.setColor(sf::Color::Cyan);
	parameterAItxt.setPosition(sf::Vector2f(640.f, 100.f));

	headlinetxt.setString("choose your gameplay!");
	headlinetxt.setFont(font);
	headlinetxt.setCharacterSize(50);
	headlinetxt.setColor(sf::Color::Cyan);
	headlinetxt.setPosition(sf::Vector2f(75.f, 75.f));

	#pragma region Tetromino sprites

	if (!redBlockTexture.loadFromFile("Assets/RedBlock_40x40.png"))
		printf("error while loading redBlockTexture");
	redBlockSprite.setTexture(redBlockTexture);

	if (!blueBlockTexture.loadFromFile("Assets/BlueBlock_40x40.png"))
		printf("error while loading blueBlockTexture");
	blueBlockSprite.setTexture(blueBlockTexture);

	if (!greenBlockTexture.loadFromFile("Assets/GreenBlock_40x40.png"))
		printf("error while loading greenBlockTexture");
	greenBlockSprite.setTexture(greenBlockTexture);

	if (!tealBlockTexture.loadFromFile("Assets/TealBlock_40x40.png"))
		printf("error while loading tealBlockTexture");
	tealBlockSprite.setTexture(tealBlockTexture);

	if (!orangeBlockTexture.loadFromFile("Assets/OrangeBlock_40x40.png"))
		printf("error while loading orangeBlockTexture");
	orangeBlockSprite.setTexture(orangeBlockTexture);

	if (!yellowBlockTexture.loadFromFile("Assets/YellowBlock_40x40.png"))
		printf("error while loading yellowBlockTexture");
	yellowBlockSprite.setTexture(yellowBlockTexture);

	if (!purpleBlockTexture.loadFromFile("Assets/PurpleBlock_40x40.png"))
		printf("error while loading purpleBlockTexture");
	purpleBlockSprite.setTexture(purpleBlockTexture);

	#pragma endregion


	levelText.setFont(font);
	levelText.setCharacterSize(40);
	levelText.setColor(sf::Color::White);
	levelText.setPosition(sf::Vector2f(500.f, 730.f));


	scoreText.setFont(font);
	scoreText.setCharacterSize(40);
	scoreText.setColor(sf::Color::White);
	scoreText.setPosition(sf::Vector2f(500.f, 560.f));

	#pragma region Tetromino preview sprites

	if (!JTetrominoPreviewTexture.loadFromFile("Assets/JTetrominoPreview.png"))
		printf("error while loading JTetrominoPreviewTexture");
	JTetrominoPreviewSprite.setTexture(JTetrominoPreviewTexture);
	JTetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!TTetrominoPreviewTexture.loadFromFile("Assets/TTetrominoPreview.png"))
		printf("error while loading TTetrominoPreviewTexture");
	TTetrominoPreviewSprite.setTexture(TTetrominoPreviewTexture);
	TTetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!OTetrominoPreviewTexture.loadFromFile("Assets/OTetrominoPreview.png"))
		printf("error while loading OTetrominoPreviewTexture");
	OTetrominoPreviewSprite.setTexture(OTetrominoPreviewTexture);
	OTetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!STetrominoPreviewTexture.loadFromFile("Assets/STetrominoPreview.png"))
		printf("error while loading STetrominoPreviewTexture");
	STetrominoPreviewSprite.setTexture(STetrominoPreviewTexture);
	STetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!ZTetrominoPreviewTexture.loadFromFile("Assets/ZTetrominoPreview.png"))
		printf("error while loading ZTetrominoPreviewTexture");
	ZTetrominoPreviewSprite.setTexture(ZTetrominoPreviewTexture);
	ZTetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!LTetrominoPreviewTexture.loadFromFile("Assets/LTetrominoPreview.png"))
		printf("error while loading LTetrominoPreviewTexture");
	LTetrominoPreviewSprite.setTexture(LTetrominoPreviewTexture);
	LTetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

	if (!ITetrominoPreviewTexture.loadFromFile("Assets/ITetrominoPreview.png"))
		printf("error while loading ITetrominoPreviewTexture");
	ITetrominoPreviewSprite.setTexture(ITetrominoPreviewTexture);
	ITetrominoPreviewSprite.setPosition(nextTetroXOffset, nextTetroYOffset);

#pragma endregion

}

void View::drawBackground()
{
	window->draw(backgroundSprt);
}

void View::drawNextTetro()
{
	int nextTetType = gameLogic->getNextTetromino()->getType();
	

	
	if (nextTetType == 0)
		window->draw(ITetrominoPreviewSprite);
	else if (nextTetType == 1)
		window->draw(OTetrominoPreviewSprite);
	else if (nextTetType == 2)
		window->draw(JTetrominoPreviewSprite);
	else if (nextTetType == 3)
		window->draw(LTetrominoPreviewSprite);
	else if (nextTetType == 4)
		window->draw(STetrominoPreviewSprite);
	else if (nextTetType == 5)
		window->draw(ZTetrominoPreviewSprite);
	else if (nextTetType == 6)
		window->draw(TTetrominoPreviewSprite);
}

void View::drawInterface()
{

	if (gameLogic->getScore() >= 10000)
		scoreText.setPosition(sf::Vector2f(460.f, 560.f));
	else if (gameLogic->getScore() >= 1000)
		scoreText.setPosition(sf::Vector2f(470.f, 560.f));
	else if (gameLogic->getScore() >= 100)
		scoreText.setPosition(sf::Vector2f(480.f, 560.f));
	else if (gameLogic->getScore() >= 10)
		scoreText.setPosition(sf::Vector2f(490.f, 560.f));


	scoreText.setString(std::to_string(gameLogic->getScore()));
	levelText.setString(std::to_string(gameLogic->getLevel()));

	window->draw(scoreText);
	window->draw(levelText);


	if (gameLogic->getFinishedAIPlays() || gameLogic->getGeneticAlgorithmComputing())
		drawAIinterface();
	
	if (gameLogic->getSinglePlayer())
		drawSinglePlayerInterface();
}

void View::drawGameOver()
{
	window->draw(gameOverSprt);
}

// draws landed array with data from gamelogic
void View::drawLandedArray()
{
	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			if (gameLogic->getLandedMatrix()[row][col] == 6)
			{
				tealBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(tealBlockSprite);
			}
			else if (gameLogic->getLandedMatrix()[row][col] == 7)
			{
				yellowBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(yellowBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 2)
			{
				purpleBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(purpleBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 5)
			{
				greenBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(greenBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 4)
			{
				redBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(redBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 3)
			{
				blueBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(blueBlockSprite);

			}
			else if (gameLogic->getLandedMatrix()[row][col] == 1)
			{
				orangeBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX, row * tileOffset + backgroundTxtOffsetY));
				window->draw(orangeBlockSprite);
			}

}

void View::drawGamePlaySelection()
{
	window->draw(headlinetxt);
	
	window->draw(singlePlayerButtonRect);
	window->draw(singlePlayertxt);
	
	window->draw(finishedAIButtonRect);
	window->draw(finishedAItxt);
	
	window->draw(geneticAlgorithmButtonRect);
	window->draw(geneticAlgorithmtxt);
}

// draws the current Tetromino with data from gamelogic
void View::drawCurrentTetromino()
{
	if (!gameLogic->getGameOver())
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 6)
				{
					tealBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(tealBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 7)
				{

					yellowBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(yellowBlockSprite);
				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 2)
				{
					purpleBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(purpleBlockSprite);
				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 5)
				{
					greenBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(greenBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 4)
				{
					redBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(redBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 3)
				{
					blueBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(blueBlockSprite);

				}
				else if (gameLogic->getCurrentTetromino()->getShape()[row][col] == 1)
				{
					orangeBlockSprite.setPosition(sf::Vector2f(gameLogic->getCurrentTetromino()->topLeft[1] * tileOffset + col * tileOffset + backgroundTxtOffsetX,
						gameLogic->getCurrentTetromino()->topLeft[0] * tileOffset + row * tileOffset + backgroundTxtOffsetY));
					window->draw(orangeBlockSprite);

				}
}

void View::drawSinglePlayerInterface()
{
	std::string info = "press D and F for rotations, space for hard drop\nand arrow keys for left / right";

	singlePlayerInfotxt.setString(info);
	window->draw(singlePlayerInfotxt);
}

void View::drawAIinterface()
{
	std::vector<float> parameters = ai->getParameters();

	float aggregateHeightParameter = parameters[0];
	float completeLinesParameter = parameters[1];
	float holesParameter = parameters[2];
	float bumpinessParameter = parameters[3];

	
	std::string game =			"\n current game:                 " + std::to_string(gameLogic->getTotalGamesPlayed())
							+ " / " + std::to_string(ai->getTotalGamesToBePlayedForOneIndividual());

	std::string individual =	"\n current individual:           " + std::to_string(ai->getCurrentIndividualIndex())
							+ " / " + std::to_string(ai->getMaxPopulation());

	std::string tetros =		"\n amount of tetrominos dropped: " + std::to_string(gameLogic->getAmountOfTetrominosDropped())
												+ " / " + std::to_string(ai->getTotalTetrominosToBeDroppedForOneIndividual());


	std::string sParameters =	"\n\n currently tried parameters: \n\n aggregate height: " + std::to_string(aggregateHeightParameter)
							+	"\n complete lines:    " + std::to_string(completeLinesParameter)
							+	"\n holes:            " + std::to_string(holesParameter)
							+	"\n bumpiness:        " + std::to_string(bumpinessParameter);

	parameterAItxt.setString(individual + game + tetros + sParameters);


	window->draw(parameterAItxt);

	/* drawing landedAI for debug reasons

	for (int row = 0; row < gameLogic->getMapHeight(); ++row)
		for (int col = 0; col < gameLogic->getMapWidth(); ++col)
			if (ai->landedAI[row][col] != 0)
			{
				tealBlockSprite.setPosition(sf::Vector2f(col * tileOffset + backgroundTxtOffsetX + 600, row * tileOffset + backgroundTxtOffsetY));
				window->draw(tealBlockSprite);
			}
	*/



}

void View::choseGamePlay(int mouseX, int mouseY)
{
	if (singlePlayerButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		gamePlayChosen = true;
		gameLogic->setGamePlay(1);
	}
	else if (finishedAIButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		gamePlayChosen = true;
		gameLogic->setGamePlay(2);
		ai->initializeAI(true);

	}
	else if (geneticAlgorithmButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		gamePlayChosen = true;
		gameLogic->setGamePlay(3);
		ai->initializeAI(true);
	}
}

void View::highlightButtons(int mouseX, int mouseY)
{
	if (singlePlayerButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		singlePlayertxt.setColor(sf::Color::Red);
	}
	else if (!singlePlayerButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		singlePlayertxt.setColor(sf::Color::Black);
	}
	if (finishedAIButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		finishedAItxt.setColor(sf::Color::Red);
	}
	else if (!finishedAIButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		finishedAItxt.setColor(sf::Color::Black);
	}
	if (geneticAlgorithmButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		geneticAlgorithmtxt.setColor(sf::Color::Red);
	}
	else if (!geneticAlgorithmButtonRect.getGlobalBounds().contains(mouseX, mouseY))
	{
		geneticAlgorithmtxt.setColor(sf::Color::Black);
	}

}


/* SETTERS / GETTERS */

void View::setGameLogic(GameLogic* gl)
{
	gameLogic = gl;
}

void View::setAI(AI* ai)
{
	this->ai = ai;
}

bool View::isGamePlayChosen()
{
	return gamePlayChosen;
}

View::~View()
{

}

