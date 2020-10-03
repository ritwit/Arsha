
#ifndef __BOARD_H__
#define __BOARD_H__

#include "global.h"

 
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
	void setBoardFromFEN(string fen);
	void printBoard();
	bool isSquareAttacked(const int r, const int f, const Color side);
	bool isPieceOccupiedSquare(const int r, const int f, const int p);
	bool isOccupiedSquare(const int r, const int f);
	bool isOffBoard(const int r, const int f);

	//Test functions
	void setBoardFromFEN_test();
	void isSquareAttacked_test();
};



#endif