#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "board.h"

class Negamax
{
private:
    Board Bd;
public:
    Negamax(){}
    Negamax(Board bd): Bd(bd) {}
    int searchDepth(const Board &bd, const int depth);
    std::tuple <Move, int> findBestMove(const Board &bd, const int depth);

    // Test functions
    void test_findBestMove();
    void test_searchDepth();
};

#endif // search.h