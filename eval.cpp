#include <iostream>
#include <cstdlib>


int Evaluation::getValue() const
{
    if(EvalMethod == MATERIAL_DIFF)
        return getMaterialDifference();
    else if
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
            ASSERT( piece >= NO_PIECE && piece <= wK);
            score += PieceVal[ piece ];
        }

    return score;
}
