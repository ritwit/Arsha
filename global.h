#ifndef __GLOBAL__H__
#define __GLOBAL__H__


#define STARTFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum Color
{
	BLACK, WHITE
};

enum Pieces
{
	NO_PIECE, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK
};

const char val2char[] = {'-' , 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};

enum Files
{
	FILEA, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH
};
#define NFILES 8

enum Ranks
{
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};
#define NRANKS 8

#endif