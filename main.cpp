#include <iostream>
#include <string>

#include "board.h"
#include "global.h"
#include "movegen.h"
#include "debug.h"


using std::cout;
using std::endl;
using std::string;


int main()
{
	cout << "Hello! I'm Arsha" << endl;
	Board board;

	//board.setBoardFromFEN_test();
	//board.isSquareAttacked_test();
	Move mv;
	mv.test_applyMove(board);

	cout << "Exiting...."<< endl;
}