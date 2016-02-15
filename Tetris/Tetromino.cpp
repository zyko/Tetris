#include "Tetromino.h"

using namespace std;

/* idea:
struct topLeft
{
	int row = 0;
	int col = 4;
};
*/

Tetromino::Tetromino(int typeOfTetromino)
{
	topLeft = { 0, 4 };
	type = typeOfTetromino;
	
	switch (typeOfTetromino)
	{
	case 0: // = 'I':
		shape = {	{ 0, 0, 1, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 1, 0 } };
		break;
	case 1: //'O':
		shape = {	{ 0, 0, 0, 0 },
					{ 0, 2, 2, 0 },
					{ 0, 2, 2, 0 },
					{ 0, 0, 0, 0 } };
		break;
	case 2: //'J':
		shape = {	{ 0, 0, 3, 0 },
					{ 0, 0, 3, 0 },
					{ 0, 3, 3, 0 },
					{ 0, 0, 0, 0 } };
		break;
	case 3: //'L':
		shape = {	{ 0, 4, 0, 0 },
					{ 0, 4, 0, 0 },
					{ 0, 4, 4, 0 },
					{ 0, 0, 0, 0 } };
		break;
	case 4: //'S':
		shape = {	{ 0, 0, 0, 0 },
					{ 0, 5, 5, 0 },
					{ 5, 5, 0, 0 },
					{ 0, 0, 0, 0 } };
		break;
	case 5: //'Z':
		shape = {	{ 0, 0, 0, 0 },
					{ 0, 6, 6, 0 },
					{ 0, 0, 6, 6 },
					{ 0, 0, 0, 0 } };
		break;
	case 6: //'T':
		shape = {	{ 0, 0, 0, 0 },
					{ 0, 7, 7, 7 },
					{ 0, 0, 7, 0 },
					{ 0, 0, 0, 0 } };
		break;
	}
}

void Tetromino::rotate(int degreeIndicator) // 1 = 90°, 3 = 270°
{
	vector < vector<int> > tmpRotate = {{ 0, 0, 0, 0 },
										{ 0, 0, 0, 0 },
										{ 0, 0, 0, 0 },
										{ 0, 0, 0, 0 } };

	for (int k = 0; k < degreeIndicator; ++k) // one loop is 90 ° rotating
	{
		// copying shape to tmpRotate for rotation
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				tmpRotate[row][col] = shape[row][col];

		//rotating
		for (int n = 0; n < 4; ++n)
			for (int i = 3; i >= 0; --i)
				shape[i][n] = tmpRotate[n][3 - i];

	}
}

vector < vector<int> > Tetromino::getShape()
{
	return shape;
}

int Tetromino::getType()
{
	return type;
}

Tetromino::~Tetromino()
{

}

