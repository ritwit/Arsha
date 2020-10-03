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
	std::cout << "Square(" << Pos[0] <<"," << Pos[1] << ")" << std::endl;
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
