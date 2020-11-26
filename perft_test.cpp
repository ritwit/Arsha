#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "global.h"
#include "board.h"
#include "movegen.h"
#include "debug.h"
#include "perft_test.h"

using std::cout;
using std::endl;

bool perftTestSuite(const std::string &file_name)
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
		int depth = 0;
		std::string::size_type pos;
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

	int test_no = 0;
	for(std::string &fen : fens)
	{

		print_separator();
		/*
		Board bd;
		bd.setBoardFromFEN(fen);
		bd.printBoard();
		*/
		int max_depth = 5;
		cout << "Testing position "<< test_no+1 <<" : " << fen << endl;

		for(int test_depth = 1; test_depth < max_depth+1; test_depth++)
		{
			auto start_time =  std::chrono::high_resolution_clock::now();
			cout << "Perft testing at depth " << test_depth << endl;
			long computed_nmoves = perftTest(fen, test_depth);
			auto end_time =  std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time- start_time); 

			if(computed_nmoves == num_moves[test_depth-1][test_no])
			{
				cout << "Leaf nodes at depth " << test_depth << " = " 
					<< computed_nmoves << endl;
				cout << "Success!" << endl;
				cout << "Total time: " << (double)duration.count()/1000000
					<< " seconds" << endl;
				cout << "Average Speed: " << (double)1000*computed_nmoves/duration.count()
					<< " kiloNodes/second" << endl; 
			}
			else
			{
				cout << "Failed test: " << test_no << endl;
				cout << "computed = " << computed_nmoves << " "
					 << "expected = " << num_moves[test_depth-1][test_no]
					 << endl;
				exit(EXIT_FAILURE);
			}

			print_separator();
		}

		test_no++;
		print_double_separator();
	}

	cout << "Successfully passed all test!" << endl;
	return true;
	
}


long perftTestDepthOne(const std::string &fen)
{
	Board bd;
	bd.setBoardFromFEN(fen);
	return perftTestDepthOne(bd);
}

long perftTestDepthOne(Board &bd)
{
	MoveGenerator mvgen;
	mvgen.clearMoves();
	//cout << "Given Board position" << endl;
	//bd.printBoard();
	mvgen.generateMoves(bd);
	//mvgen.printAllMovesGenerated(bd);
	long num_moves = mvgen.getNumMoves();
	//cout << "Number of legal moves: " << num_moves << endl;
	return num_moves;
}


long perftTest(const std::string &fen, int depth)
{
	Board bd;
	bd.setBoardFromFEN(fen);

	return numMovesAtDepth(bd, depth);
}

long numMovesAtDepth(Board &bd, int depth)
{
	if(depth == 1)
		return perftTestDepthOne(bd);

	MoveGenerator mvgen;
	mvgen.generateMoves(bd);
	long num_moves = 0;
	for(Move &mv: mvgen.MoveList)
	{
		Board bdt = bd;
		mv.applyMove(bdt);
		num_moves += numMovesAtDepth(bdt, depth-1);
	}

	return num_moves;
}