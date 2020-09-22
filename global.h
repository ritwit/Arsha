#ifndef __GLOBAL__H__
#define __GLOBAL__H__


#define STARTFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TESTFEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define TESTFEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define TESTFEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

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
#define NFILESEXT 10

enum Ranks
{
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};
#define NRANKS 8
#define NRANKSEXT 12
#define OFFBRD 0

#endif