#ifndef __EVAL_H__
#define __EVAL_H__

#include "board.h"

#define MATERIAL_DIFF 1

const int PieceValues[13] =
{
    0,          //NO_PIECE
    -100,       //bP
    -300,       //bN
    -310,       //bB
    -550,       //bR
    -960,       //bQ
    -10000,     //bK
    +100,       //wP
    +300,       //wN
    +310,       //wB
    +550,       //wR
    +960,       //wQ
    +10000      //wK
};

class Evaluation
{
public:
    Board Bd;
    int EvalMethod;

public:
    Evaluation(){}
    Evaluation(const Board &bd,
               const int eval_method):
               Bd(bd),
               EvalMethod(eval_method)
               {}

    int getValue() const;

    void test_getMaterialDifference();

private:
    int getMaterialDifference() const;

};

#endif