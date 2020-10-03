#ifndef __BOARD_H__
#define __BOARD_H__

#include "global.h"
#include "square.h"

class Board
{
private:
	
	//standard 8x8 board
	short Brd[NRANKS][NFILES] = {0};

	//Castle Permissions wK wQ bK bQ
	short Castle[4] = {0, 0, 0, 0};

	// Side to play
	Color ActiveColor;

	// En Passant Decimal format rank|file
	short EnP;

	// Halfmoves
	short HalfMoves = 0;

	// Total moves
	short NMoves = 0;

public:
	void setBoardFromFEN(const std::string fen);
	void printBoard() const;
	bool isSquareAttacked(const Square &sq, const Color side) const;
	bool isPieceOccupiedSquare(const Square &sq, const int p) const;
	bool isOccupiedSquare(const Square &sq) const;
	bool isOffBoard(const Square &sq) const ;
	int getSquareValue(const Square &sq) const;

	//Test functions
	void setBoardFromFEN_test();
	void isSquareAttacked_test();
};



#endif