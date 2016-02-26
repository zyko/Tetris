#pragma once

#include "Common.h"

#include "GameLogic.h"

#include <string>

#include <SFML/Graphics.hpp>


class View
{
private:

	// variables

	bool gamePlayChosen;


	std::vector < std::vector<int> > nextTetShape;


	sf::RenderWindow* window;

	GameLogic* gameLogic;
	AI* ai;

	sf::Texture backgroundTxt;
	sf::Sprite backgroundSprt;

	sf::Texture gameOverTxt;
	sf::Sprite gameOverSprt;


	sf::Sprite tealBlockSprite;
	sf::Sprite yellowBlockSprite;
	sf::Sprite purpleBlockSprite;
	sf::Sprite greenBlockSprite;
	sf::Sprite redBlockSprite;
	sf::Sprite blueBlockSprite;
	sf::Sprite orangeBlockSprite;

	sf::Texture redBlockTexture;
	sf::Texture blueBlockTexture;
	sf::Texture tealBlockTexture;
	sf::Texture orangeBlockTexture;
	sf::Texture yellowBlockTexture;
	sf::Texture purpleBlockTexture;
	sf::Texture greenBlockTexture;

	sf::Sprite JTetrominoPreviewSprite;
	sf::Sprite ITetrominoPreviewSprite;
	sf::Sprite LTetrominoPreviewSprite;
	sf::Sprite STetrominoPreviewSprite;
	sf::Sprite TTetrominoPreviewSprite;
	sf::Sprite OTetrominoPreviewSprite;
	sf::Sprite ZTetrominoPreviewSprite;

	sf::Texture JTetrominoPreviewTexture;
	sf::Texture ITetrominoPreviewTexture;
	sf::Texture LTetrominoPreviewTexture;
	sf::Texture STetrominoPreviewTexture;
	sf::Texture TTetrominoPreviewTexture;
	sf::Texture OTetrominoPreviewTexture;
	sf::Texture ZTetrominoPreviewTexture;

	sf::Font font;
	sf::Text parameterAItxt;

	sf::Text singlePlayerInfotxt;

	sf::Texture buttonTexture;

	sf::RectangleShape singlePlayerButtonRect;
	sf::RectangleShape finishedAIButtonRect;
	sf::RectangleShape geneticAlgorithmButtonRect;

	sf::Text singlePlayertxt;
	sf::Text finishedAItxt;
	sf::Text geneticAlgorithmtxt;

	sf::Text headlinetxt;

	sf::Text levelText;
	sf::Text scoreText;

	const float tileOffset = 40.f;
	const float backgroundTxtOffsetX = 30.f;
	const float backgroundTxtOffsetY = -50.f;
	const float nextTetroXOffset = 460.f;
	const float nextTetroYOffset = 105.f;

	// functions

	void drawAIinterface();

public:

	// variables

	// functions

	void drawSinglePlayerInterface();
	void drawGameOver();
	void drawBackground();
	void drawInterface();
	void drawNextTetro();
	void drawGamePlaySelection();
	void drawLandedArray();

	void drawCurrentTetromino();

	void choseGamePlay(int mouseX, int mouseY);
	void highlightButtons(int mouseX, int mouseY);

	// setters

	void setGameLogic(GameLogic* gl);
	void setAI(AI* ai);

	// getters

	bool isGamePlayChosen();

	// constructor / destructor

	View(sf::RenderWindow* window);
	~View();
};