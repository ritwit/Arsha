#ifndef __GLOBAL__H__
#define __GLOBAL__H__


#define STARTFEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define TESTFEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define TESTFEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define TESTFEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

enum Color : bool
{
	BLACK, WHITE
};

enum Pieces
{
	NO_PIECE, bP, bN, bB, bR, bQ, bK, wP, wN, wB, wR, wQ, wK
};
#define NPIECES 13

#define NO_COLOR 3
const int PieceSide[13] = { NO_COLOR, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
                             WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
                           };

#define MAXDIR 8
const int AttackDir[NPIECES][MAXDIR][2] =
{
	{{0}                                   }, //NO_PIECE
	{{-1, -1}, {-1, +1}                    }, //bP
	{{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1},
     {-1, -2}, {-1, +2}, {+1, -2}, {+1, +2}}, //bN
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}}, //bB
    {{-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bR
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bQ
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bK
    {{+1, -1}, {+1, +1}                    }, //wP
    {{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1},
     {-1, -2}, {-1, +2}, {+1, -2}, {+1, +2}}, //wN
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}}, //wB
    {{-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wR
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wQ
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wK

};
const short NAttackDir[13] = 
{0, 2, 8, 4, 4, 8, 8, 2, 8, 4, 4, 8, 8};

// Bishops, Rooks and Queen have ranged attack
const bool Ranged[13] = {false, false, false,  true,  true, true, false,
                   false, false,  true,  true,  true, false};

const char val2char[] = {'-' , 'p', 'n', 'b', 'r', 'q', 'k', 'P', 'N', 'B', 'R', 'Q', 'K'};

enum Files
{
	FILEA, FILEB, FILEC, FILED, FILEE, FILEF, FILEG, FILEH
};
#define NFILES 8
const char VAL2FILE[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

enum Ranks
{
	RANK1, RANK2, RANK3, RANK4, RANK5, RANK6, RANK7, RANK8
};
#define NRANKS 8
const char VAL2RANK[] = {'1', '2', '3', '4', '5', '6', '7', '8'};


#define OFFBRD 998

#endif