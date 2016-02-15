#pragma once
#include <vector>

class Tetromino
{
private:
	//void blabla();

	std::vector < std::vector<int> > shape;
	int type;

public:

	
	std::vector <int> topLeft;
	std::vector < std::vector<int> > getShape();
	void rotate(int degreeIndicator);
	
	int getType();

	Tetromino(int typeOfTetromino);
	~Tetromino();
};