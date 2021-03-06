#include <iostream>
#include <string>

#include "square.h"
#include "global.h"

using std::string;

void Square::setSquare(const int rank, const int file)
{
	*this = Square(rank, file);
}

void Square::getSquare(int &rank, int &file) const
{
	rank = Pos[0];
	file = Pos[1];
}

void Square::printSquare() const
{
	if(!this->isOffBoard())
	{	std::cout << "Square(" << Pos[1] <<","
		 << Pos[0] << ")" << std::endl;
	}
	else
		std::cout << "EMPTY" << std::endl;
}



bool Square::isEqual(const Square &sq ) const
{
	if (this->Pos[0] == sq.Pos[0] && this->Pos[1] == sq.Pos[1])
		return true;
	return false;
}


bool Square::isOffBoard() const
{
	if (Pos[0] < 0 || Pos[0] > 7 || Pos[1]  < 0 || Pos[1] > 7)
		return true;
	return false;
}

string Square::getString() const
{
	if(!this->isOffBoard())
	{
		string rank(1, VAL2RANK[Pos[0]]);
		string file(1, VAL2FILE[Pos[1]]);
		return file + rank;
	}
	else
	{
		string empty = "EMPTY";
		return empty;
	}
}