#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "global.h"
#include "board.h"
#include "movegen.h"
#include "debug.h"
#include "perft_test.h"

using std::cout;
using std::endl;

bool perftTestSuite(std::string file_name)
{
	std::ifstream perft_file;
	perft_file.open(file_name, std::ios::in);

	if (!perft_file)
	{
		ERROR("Unable to open file " << file_name);
		exit(EXIT_FAILURE);
	}

	std::string line;
	std::vector<std::string> fens;
	std::vector<long> num_moves[6];

	while(getline(perft_file, line))
	{
		// add fen string
		std::string fen = line.substr(0, line.find(';'));
		line.erase(0, line.find(';') + 1);
		fens.push_back(fen);

		//get number of moves
		std::string subline;
		int pos, depth = 0;
		while((pos = line.find(';')) != std::string::npos)
		{
			subline = line.substr(0, pos);
			line.erase(0, pos+1);
			
			//add nmoves at depth to vector num_moves
			subline.erase(0, subline.find(' ') + 1);
			num_moves[depth].push_back(std::stoi(subline));
			depth++;
		}

		//get depth 6
		subline = line.substr(0, pos);
		subline.erase(0, subline.find(' ') + 1);
		num_moves[depth].push_back(std::stol(subline));
	}

	perft_file.close();

	MoveGenerator mvgen;
	int test_no = 0;
	for(std::string &fen : fens)
	{
		print_separator();
		cout << "Testing position: " << fen << endl;
		long computed_nmoves = mvgen.perftTestDepthOne(fen);

		if(computed_nmoves == num_moves[0][test_no])
		{
			cout << "Passed test: " << test_no+1 << endl << endl; 
		}
		else
		{
			cout << "Failed test: " << test_no << endl;
			cout << "computed = " << computed_nmoves << " "
				 << "expected = " << num_moves[0][test_no]
				 << endl;
			exit(EXIT_FAILURE);
		}

		test_no++;
		print_separator();
	}
	

}