#include <iostream>
#include <string>

#include "board.h"
#include "global.h"


using std::cout;
using std::endl;
using std::string;


int main()
{
	cout << "Hello! I'm Arsha" << endl;
	Board board;
	board.setBoardFromFEN_test();
	//board.isSquareAttacked_test();
	//board.initExtBrd();
	//board.initExtBrd_test();
	cout << "Exiting"<< endl;
}