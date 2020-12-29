#include <iostream>
#include <cstdlib>

#include "eval.h"
#include "debug.h"

using std::cout;
using std::endl;


int Evaluation::getValue() const
{
    if(EvalMethod == MATERIAL_DIFF)
        return getMaterialDifference();
    else
    {
        std::cerr << "Not Implemented" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int Evaluation::getMaterialDifference() const
{
    int score = 0;

    for(int i = RANK1; i < RANKMAX; i++)
        for(int j = FILEA; j < FILEMAX; j++)
        {
            int piece = Bd.Brd[i][j];
            ASSERT(piece >= NO_PIECE && piece <= wK);
            score += PieceValues[ piece ];
        }

    return score;
}

void Evaluation::test_getMaterialDifference()
{
    Board bd;
    bd.setBoardFromFEN("4k3/8/8/8/8/8/8/4K2r w K - 0 1");
    *this = Evaluation(bd, MATERIAL_DIFF);
    cout << "score at position:" << endl;
    Bd.printBoard();
    cout << "Score: " << getValue() << endl;
}
