#include <iostream>

#include "square.h"


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

void Square::moveSquare(const int dir1, const int dir2)
{
	Pos[0] += dir1;
	Pos[1] += dir2;
}

void Square::moveSquare(const int dir[2])
{
	Pos[0] += dir[0];
	Pos[1] += dir[1];
}

void Square::moveSquareOpposite(const int dir[2])
{
	Pos[0] -= dir[0];
	Pos[1] -= dir[1];
}

void Square::moveSquareOpposite(const int dir1, const int dir2)
{
	Pos[0] -= dir1;
	Pos[1] -= dir2;
}

bool Square::isEqual(const Square &sq ) const
{
	if (this->Pos[0] == sq.Pos[0] && this->Pos[1] == sq.Pos[1])
		return true;
	return false; 
}

bool Square::isOffSquare() const
{
	if (Pos[0] == OFFBRD && Pos[1] == OFFBRD)
		return true;
	return false;
}

bool Square::isOffBoard() const
{
	if (Pos[0] < 0 || Pos[0] > 7 || Pos[1]  < 0 || Pos[1] > 7)
		return true;
	return false; 
}
