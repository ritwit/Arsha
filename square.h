#ifndef __SQUARE_H__
#define __SQUARE_H__

class Square
{

public:
	int Pos[2];

public:

	Square(const int rank, int const file)
		: Pos{rank, file}{}
	Square()
		: Square(99,99){}
	Square(const int a[])
		{Pos[0] = a[0]; Pos[1] = a[1];}
	Square(const Square &sq)
		{Pos[0] = sq.Pos[0]; Pos[1] = sq.Pos[1];}

	void setSquare(const int rank, const int file);
	void getSquare(int &rank, int &file) const;
	void printSquare() const;
	void moveSquare(const int dir1, const int dir2);
	void moveSquare(const int dir[2]);
	void moveSquareOpposite(const int dir[2]);
	void moveSquareOpposite(const int dir1, const int dir2);
	bool isEqual(const Square &sq ) const;

};



#endif