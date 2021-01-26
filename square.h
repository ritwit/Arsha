#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <string>
#include "global.h"

class Square
{

public:
	int Pos[2];

public:

	Square(const int rank, const int file)
		: Pos{rank, file}{}
	Square()
		: Square(OFFBRD,OFFBRD){}
	Square(const int a[])
		{Pos[0] = a[0]; Pos[1] = a[1];}
	Square(const Square &sq)
		{Pos[0] = sq.Pos[0]; Pos[1] = sq.Pos[1];}

	void setSquare(const int rank, const int file);
	void getSquare(int &rank, int &file) const;
	bool isEqual(const Square &sq ) const;
	bool isOffBoard() const;
	void printSquare() const;
	std::string getString() const;

	inline void moveSquare(const int dir1, const int dir2)
	{
		Pos[0] += dir1;
		Pos[1] += dir2;
	}
	inline void moveSquare(const int dir[2])
	{
		Pos[0] += dir[0];
		Pos[1] += dir[1];
	}
	inline void moveSquareOpposite(const int dir[2])
	{
		Pos[0] -= dir[0];
		Pos[1] -= dir[1];
	}
	inline void moveSquareOpposite(const int dir1, const int dir2)
	{
		Pos[0] -= dir1;
		Pos[1] -= dir2;
	}
	inline bool isOffSquare() const
	{
		if (Pos[0] == OFFBRD && Pos[1] == OFFBRD)
			return true;
		return false;
	}

};

static const Square OFFSQ (OFFBRD, OFFBRD);

#endif