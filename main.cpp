#include <iostream>
#include <string>

#include "board.h"
#include "global.h"


using std::cout;
using std::endl;
using std::string;


int main(int argc, char const *argv[])
{
	cout << "Hello! I'm Arsha" << endl;
	Board board;
	//board.setBoardFromFEN_test();
	board.initExtBrd();
	board.initExtBrd_test();
	cout << "Exiting"<< endl;
}