#include <iostream>
#include <string>

#include "board.h"
#include "eval.h"
#include "global.h"
#include "movegen.h"
#include "debug.h"
#include "perft_test.h"


using std::cout;
using std::endl;
using std::string;




int main()
{
	cout << "Hello! I'm Arsha" << endl;
	Board board;

	//board.setBoardFromFEN_test();
	//board.isSquareAttacked_test();
	//Move mv;
	//mv.test_applyMove(board);
	//MoveGenerator mvgen;
	//string fen = "8/8/8/8/8/8/6k1/4K2R w K - 0 1";
	//mvgen.perftTestDepthOne(fen);

	//string file_name = "perftsuite.epd";
	//bool flag = perftTestSuite(file_name);
	Evaluation ev;
	ev.test_getMaterialDifference();

	cout << "Exiting...."<< endl;
}