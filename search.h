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
    int searchDepth(const Board bd, const int depth);

    void test_searchDepth();
};

#endif // search.h