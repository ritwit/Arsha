
#ifndef __BOARD_H__
#define __BOARD_H__

#include "global.h"

using std::cout;
using std::endl;
using std::string;


class Board
{

private:
	static short ExtBrd[NRANKSEXT][NFILESEXT]; 
	
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
	void initExtBrd();
};



#endif