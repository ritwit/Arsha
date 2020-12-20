#ifndef __EVAL_H__
#define __EVAL_H__

#define MATERIAL_DIFF 1

int PieceValues[]
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
}

class Evaluation
{
public:
    Board Bd;
    int EvalMethod;

public:
    Evaluation(const Board &bd,
               const int eval_method):
               bd(bd),
               eval_method(eval_method)
               {}

    int getValue() const;

private:
    int getMaterialDifference() const;

}

#endif