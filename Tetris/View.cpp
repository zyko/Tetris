#include "View.h"

View::View(sf::RenderWindow* window)
{
	this->window = window;


	sf::Texture gameOverTxt;
	if (!gameOverTxt.loadFromFile("Assets/gameOver.png"))
		printf("error while loading gameOver.png \n");
	gameOverSprt.setTexture(gameOverTxt);


	if (!font.loadFromFile("Fonts/courbd.ttf")) // this is courier new bold
		printf("error while loading font \n");
	parameterAItxt.setFont(font);
	parameterAItxt.setCharacterSize(20);
	parameterAItxt.setColor(sf::Color::Green);
	parameterAItxt.setPosition(sf::Vector2f(640.f, 100.f));

	#pragma region Tetromino sprites

	sf::Texture redBlockTexture;
	if (!redBlockTexture.loadFromFile("Assets/RedBlock_40x40.png"))
		printf("error while loading redBlockTexture");
	redBlockSprite.setTexture(redBlockTexture);

	sf::Texture blueBlockTexture;
	if (!blueBlockTexture.loadFromFile("Assets/BlueBlock_40x40.png"))
		printf("error while loading blueBlockTexture");
	blueBlockSprite.setTexture(blueBlockTexture);

	sf::Texture greenBlockTexture;
	if (!greenBlockTexture.loadFromFile("Assets/GreenBlock_40x40.png"))
		printf("error while loading greenBlockTexture");
	greenBlockSprite.setTexture(greenBlockTexture);

	sf::Texture tealBlockTexture;
	if (!tealBlockTexture.loadFromFile("Assets/TealBlock_40x40.png"))
		printf("error while loading tealBlockTexture");
	tealBlockSprite.setTexture(tealBlockTexture);

	sf::Texture orangeBlockTexture;
	if (!orangeBlockTexture.loadFromFile("Assets/OrangeBlock_40x40.png"))
		printf("error while loading orangeBlockTexture");
	orangeBlockSprite.setTexture(orangeBlockTexture);

	sf::Texture yellowBlockTexture;
	if (!yellowBlockTexture.loadFromFile("Assets/YellowBlock_40x40.png"))
		printf("error while loading yellowBlockTexture");
	yellowBlockSprite.setTexture(yellowBlockTexture);

	sf::Texture purpleBlockTexture;
	if (!purpleBlockTexture.loadFromFile("Assets/PurpleBlock_40x40.png"))
		printf("error while loading purpleBlockTexture");
	purpleBlockSprite.setTexture(purpleBlockTexture);

	#pragma endregion

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

	drawAIinterface();
	
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


	std::string a = "\n\n currently tried parameters: \n";
	std::string b = "\n aggregate height: ";
	std::string c = std::to_string(aggregateHeightParameter);
	std::string d = "\n complete lines:    ";
	std::string e = std::to_string(completeLinesParameter);
	std::string f = "\n holes:            ";
	std::string g = std::to_string(holesParameter);
	std::string h = "\n bumpiness:        ";
	std::string i = std::to_string(bumpinessParameter);

	parameterAItxt.setString(individual + game + tetros + a + b + c + d + e + f + g + h + i);


	window->draw(parameterAItxt);

}

void View::setGameLogic(GameLogic* gl)
{
	gameLogic = gl;
}

void View::setAI(AI* ai)
{
	this->ai = ai;
}

View::~View()
{

}

