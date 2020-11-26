#ifndef __MOVEGEN_H__
#define __MOVEGEN_H__

#include "global.h"

enum CastleSide
{
	NOCASTLE, BKINGSIDE, BQUEENSIDE, WKINGSIDE, WQUEENSIDE
};


enum PawnJump
{
	NOPJUMP, PJUMP
};

#define MVMAXDIR 8
const int MoveDir[NPIECES][MAXDIR][2] =
{
	{{0}                                   }, //NO_PIECE
	{{-1,  0}                              }, //bP
	{{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1},
     {-1, -2}, {-1, +2}, {+1, -2}, {+1, +2}}, //bN
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}}, //bB
    {{-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bR
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bQ
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //bK
    {{+1,  0}                              }, //wP
    {{-2, -1}, {-2, +1}, {+2, -1}, {+2, +1},
     {-1, -2}, {-1, +2}, {+1, -2}, {+1, +2}}, //wN
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}}, //wB
    {{-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wR
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wQ
    {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1},
     {-1,  0}, {+1 , 0}, { 0, -1}, { 0, +1}}, //wK

};
const short NMovedir[13] = 
{0, 1, 8, 4, 4, 8, 8, 1, 8, 4, 4, 8, 8};

class Move
{
public:
    Square From;
    Square To;
    int Piece;
    int Capture;
    Square Enp;
    int Promotion;
    int Castle;
    int PawnJump;
    
public:
    void printMove() const;
    Move() {}
    Move(const Square From1,
         const Square To1,
         const int piece,
         const int Capture = NO_PIECE, /*NO_PIECE if EnP capture */
         const Square Enp = OFFSQ,/*To and Enp are same! replace by int? Need to change capture in pawn move gen*/
         const int Promotion = NO_PIECE,
         const int Castle = NOCASTLE,
         const int PawnJump = NOPJUMP)
         :From(From1),
          To(To1),
          Piece(piece),
          Capture(Capture),
          Enp(Enp),
          Promotion(Promotion), 
          Castle(Castle),
          PawnJump(PawnJump) { }
    void applyMove(Board &bd) const;

    // Test code
    void test_applyMove(Board &bd);

private:
	void applyCastleMove(Board &bd) const;
	Square getCaptureEnpSquare() const;
	Square generateEnpSquare() const;
    void updateCastlePermissions(Board &bd) const;
};

class MoveGenerator
{
public:
    std::vector<Move> MoveList;

public:
    void generateMoves(const Board &bd);
    //void applyMove(Board &bd) const;
    void printMove() const;
    int getNumMoves() const;

    void test_addPawnMoves(Board &bd);
    void test_addCastleMoves(Board &bd);
    void test_generateMoves(Board &bd);

    void printAllMovesGenerated(Board &bd);
    void clearMoves();

private:
    void removeIllegalMoves(const Board &bd);
    void addCastleMoves(const Board &bd);
    void addPawnMoves(const Board &bd, const Square &sq, const int &piece);
    bool isPromotion(const int &piece, const Square &to);
    void addPromotionMoves(const Square &sq, const Square &single_jump,
        const int piece, const int capture);
};


#endif