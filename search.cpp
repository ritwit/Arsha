#include <iostream>
#include <tuple>

#include "debug.h"
#include "eval.h"
#include "global.h"
#include "movegen.h"
#include "search.h"

using std::cout;
using std::endl;

#define HIGH_SCORE 10000000

std::tuple <Move, int> Negamax::findBestMove(const Board &bd, const int depth)
{
    ASSERT(depth > 0);
    int best_score = -HIGH_SCORE;
    Move best_move;

    MoveGenerator mvgen;
    mvgen.generateMoves(bd);

    for(Move &mv: mvgen.MoveList)
    {
        Board bdt = bd;
        mv.applyMove(bdt);
        int score = -searchDepth(bdt, depth - 1);
        if(score > best_score)
        {
            best_score = score;
            best_move = mv;
        }
    }
    return std::make_tuple(best_move, best_score);
}

int Negamax::searchDepth(const Board &bd, const int depth)
{
    int active_color = bd.ActiveColor;
    int best_score = - HIGH_SCORE;

    // Return score s.t. more positive score is better
    // for the current player
    if(depth == 0)
    {
        Evaluation ev(bd, MATERIAL_DIFF);
        //int score = ev.getValue();
        int score = active_color == WHITE ?
                    ev.getValue(): -ev.getValue();
        return score;
    }

    MoveGenerator mvgen;
    mvgen.generateMoves(bd);

    for(Move &mv: mvgen.MoveList)
    {
        Board bdt = bd;
        mv.applyMove(bdt);
        int score = -searchDepth(bdt, depth - 1);
        if(score > best_score)
            best_score = score;
    }
    return best_score;

}

void Negamax::test_searchDepth()
{
    int max_depth = 7;
    Board bd;
    bd.setBoardFromFEN("1K6/1P6/8/8/8/8/5r2/6k1 w - - 0 1 ");
    cout << "Testing Negamax" << endl;

    for(int depth = 0; depth < max_depth; depth++)
    {
        int score = searchDepth(bd, depth);
        cout << "At depth: " << depth
             << " Score: " << score << endl;
    }
}

void Negamax::test_findBestMove()
{
    int max_depth = 3;
    Board bd;
    bd.setBoardFromFEN("5k2/2n5/4n3/8/1p6/2P5/4RPPP/5K2 w - - 0 1");
    cout << "Testing findBestMove" << endl;

    for(int depth = 1; depth < max_depth; depth++)
    {
        Move best_move;
        int score;
        std::tie(best_move, score) = findBestMove(bd, depth);
        cout << "At depth: " << depth
             << " Score: " << score << endl
             << "Best Move" << endl;
        best_move.printMove();
        cout << endl;
    }

}
