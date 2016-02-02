#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;


//remember:
// row = X = mapHeight = 22;
// column = Y = mapWidth = 10

struct tetrominoTryAbstract
{
	int typeOftetromino;
	int shapeSizeRow;
	int shapeSizeColumn;
	int bar;
	vector < int > topleft = { 0, 4 };

	/*
	vector < vector<int> > shape = {	vector <int> { 0, 0, 0, 0 },
										vector <int> { 0, 0, 0, 0 },
										vector <int> { 0, 0, 0, 0 },
										vector <int> { 0, 0, 0, 0 } };
	*/


	vector < vector<int> > shape;

	

	//shape.resize(a, vector<int>(b, initialValue));

	/*
	shape = {	{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 } };

	shape[0] = vector<int>(0, 0, 0, 0);
	*/

	tetrominoTryAbstract() : bar(3)
	{
		//vector < int > topleft = { 0, 4 };

		//vector < vector<int> > shape(4, vector<int>(4));

		int a = 4;
		int b = 4;
		int initialValue = 0;

		shape.resize(a, vector<int>(b, initialValue));

		switch (typeOftetromino)
		{
			case 0: // = 'I': // supposed to be: 4, 1
				shape = {	{ 0, 0, 1, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 1, 0 } };
				break;
			case 1: //'O': // supposed to be: 2, 2
				shape = {	{ 0, 0, 0, 0 },
							{ 0, 1, 1, 0 },
							{ 0, 1, 1, 0 },
							{ 0, 0, 0, 0 } };
				break;
			case 2: //'J':
				shape = {	{ 0, 0, 1, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 1, 1, 0 },
							{ 0, 0, 0, 0 } };
				break;
			case 3: //'L':
				shape = {	{ 0, 1, 0, 0 },
							{ 0, 1, 0, 0 },
							{ 0, 1, 1, 0 },
							{ 0, 0, 0, 0 } };
				break;
			case 4: //'S':
				shape = {	{ 0, 0, 0, 0 },
							{ 0, 1, 1, 0 },
							{ 1, 1, 0, 0 },
							{ 0, 0, 0, 0 } };
				break;
			case 5: //'Z':
				shape = {	{ 0, 0, 0, 0 },
							{ 0, 1, 1, 0 },
							{ 0, 0, 1, 1 },
							{ 0, 0, 0, 0 } };
				break;
			case 6: //'T':
				shape = {	{ 0, 0, 0, 0 },
							{ 0, 1, 1, 1 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 0, 0 } };
				break;

		}

	}
};

// trying stuff, prolly wont need it anymore
struct tetrominoI
{
	vector < vector<int> > shape;

	int size;
	int height;
};
struct tetrominoO
{
	vector < vector<int> > shape;


	int size;
	int height;
};
struct tetrominoJ
{
	vector < vector<int> > shape;


	int size;
	int height;
};
struct tetrominoL
{
	vector < vector<int> > shape;


	int size;
	int height;
};
struct tetrominoS
{
	vector < vector<int> > shape;


	int size;
	int height;
};
struct tetrominoZ
{
	vector < vector<int> > shape;


	int size;
	int height;
};
struct tetrominoT
{
	vector < vector<int> > shape;


	int size;
	int height;
};
// <<<<



int mapHeight = 22;
int mapWidth = 10;

vector< vector<int> > landed( mapHeight, vector<int>(mapWidth) );


int main()
{
	sf::Clock clock; // starts the clock
	sf::RenderWindow window(sf::VideoMode(800, 600), "My window"); //sf::Style::None);
	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);




#pragma region text stuff / aint workin
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("comic.ttf"))
	{
		//error
	}
	string s = "hello world";
	text.setCharacterSize(50);
	text.setString("hello world");
	text.setColor(sf::Color::White);
#pragma endregion

	// creating tmp tetromino
	tetrominoTryAbstract *tmp = {};
	tmp->typeOftetromino = 1;

	// initializing landed array with 0s
	for (int i = 0; i < mapHeight; ++i)
		for (int n = 0; n < mapWidth; ++n)
			landed[i][n] = 0;

	// filling bottom line with 1s
	for (int i = 0; i < mapWidth; ++i)
	{
		landed[mapHeight-1][i] = 1;
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		
		
		

		window.clear();

		
		// what is to be shown in every "frame":


		/* 
		so every frame we need to draw:
		landed array
		my current tetromino
		background constant stuff
		*/

		// drawing landed array:
		for (int row = 0; row < mapHeight; ++row)
		{
			for (int col = 0; col < mapWidth; ++col)
			{
				if (landed[row][col] != 0)
				{
					//insert drawing stuff here
				}
			}
		}

		// creating current tetromino "tmp"
		

		int asdf = tmp->shape.size();
		int blabla = tmp->shape[0].size();

		// drawing current tetromino:
		for (int row = 0; row < asdf; ++row)
		{
			for (int col = 0; col < blabla; ++col)
			{
				if (landed[row][col] != 0)
				{
					//total stuff = 
					//tmp.topleft[0] + row; 
					//tmp.topleft[1] + col; 
				}
			}
		}

		// check for possible collisionsrres

		// check for complete lines

		window.draw(text);
		//window.draw(shape);
		
		
		
		window.display();


	}

	return 0;
}