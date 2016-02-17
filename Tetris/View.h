#pragma once

#include "Common.h"

#include "GameLogic.h"


#include <SFML/Graphics.hpp>


class View
{
private:

	// variables

	std::vector < std::vector<int> > nextTetShape;


	sf::RenderWindow* window;
	sf::Sprite gameOverSprt;

	GameLogic* gameLogic;



	sf::Sprite tealBlockSprite;
	sf::Sprite yellowBlockSprite;
	sf::Sprite purpleBlockSprite;
	sf::Sprite greenBlockSprite;
	sf::Sprite redBlockSprite;
	sf::Sprite blueBlockSprite;
	sf::Sprite orangeBlockSprite;

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



	const float tileOffset = 40.f;
	const float backgroundTxtOffsetX = 30.f;
	const float backgroundTxtOffsetY = -50.f;
	const float nextTetroXOffset = 460.f;
	const float nextTetroYOffset = 105.f;

	// functions

public:

	// variables

	// functions

	void drawNextTetro();

	// setters

	void setGameLogic(GameLogic* gl);

	// constructor / destructor

	View(sf::RenderWindow* window);
	~View();
};