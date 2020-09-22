#include <iostream>
#include <string>

#include "board.h"

short Board::ExtBrd[NRANKSEXT][NFILESEXT];

void Board::printBoard()
{
	cout << "Printing Chess Board..." << endl << endl;
	cout <<"\t";

	for(int f = FILEA; f <= FILEH; f++)
	{
		char name = 'A' + f;
		cout << "FILE " << name <<"\t";
	}

	cout << endl << endl;

	for(int r = RANK8; r >= RANK1; r--)
	{
		cout << "RANK " << r+1 <<"\t";
		
		for(int f = FILEA; f <= FILEH; f++)
		{
			cout << val2char[Brd[r][f]] << "\t";
		}
		
		cout << endl << endl;
	}

	if (ActiveColor == WHITE)
		cout<< "Side to play: WHITE"  << endl;
	if (ActiveColor == BLACK)
		cout<< "Side to play: BLACK" << endl;

	cout << "Total Moves:" << NMoves << endl;
	cout << "Half Moves:"  << HalfMoves << endl;
	cout << "En Passant Square:" << EnP << endl;

}

void Board::setBoardFromFEN(string fen)
{
	int rank = RANK8;
	int file = FILEA;

	//
	char enp_file = '\0';

	// Different parts of the FEN format
	int section = 0;

	for (auto c = fen.begin(); c != fen.end(); c++)
	{
		if(*c == ' ')
		{
			section++;
			continue;
		}

		if (section == 0)
		switch(*c)
		{
			// BLACK
			case 'r':
				Brd[rank][file] = bR;
				file++;
				break;

			case 'n':
				Brd[rank][file] = bN;
				file++;
				break;

			case 'b':
				Brd[rank][file] = bB;
				file++;
				break;

			case 'q':
				Brd[rank][file] = bQ;
				file++;
				break;

			case 'k':
				Brd[rank][file] = bK;
				file++;
				break;

			case 'p':
				Brd[rank][file] = bP;
				file++;
				break;

			//WHITE
			case 'R':
				Brd[rank][file] = wR;
				file++;
				break;

			case 'N':
				Brd[rank][file] = wN;
				file++;
				break;

			case 'B':
				Brd[rank][file] = wB;
				file++;
				break;

			case 'Q':
				Brd[rank][file] = wQ;
				file++;
				break;

			case 'K':
				Brd[rank][file] = wK;
				file++;
				break;

			case 'P':
				Brd[rank][file] = wP;
				file++;
				break;

			case '/':
				file = FILEA;
				rank--;
				break;

			default:
				int skip = *c - '0';
				for (int s = 0; s < skip; s++)
				{
					Brd[rank][file] = NO_PIECE;
					file++;
				}
		}//end switch


		else if (section == 1)
		{	
			if (*c == 'w')
				ActiveColor = WHITE;
			if (*c == 'b')
				ActiveColor = BLACK;
		}

		else if (section == 2)
		{
			switch(*c)
			{
				case 'K':
					Castle[0] = 1;
					break;
				case 'Q':
					Castle[1] = 1;
					break;
				case 'k':
					Castle[2] = 1;
					break;
				case 'q':
					Castle[3] = 1;
					break;
				default:
					cout << "ERROR" << endl;
			}
		}

		else if (section == 3)
		{
			if( *c == '-')
			{
				EnP = 0;
				continue;
			}
			
			if (!enp_file)
			{
				enp_file = *c;
				continue;
			}

			int enp_file_no = enp_file - 'a';
			int enp_rank_no = *c - '1';

			EnP = 10*enp_rank_no + enp_file_no;

		}

		else if (section == 4)
			HalfMoves = *c - '0';

		else if (section == 5)
			NMoves = *c - '0';

		else
			cout << "ERROR" << endl;

	}//end for loop over fen string
	//#ifdef DEBUG
	//	printBoard();
	//#endif
}

void Board::initExtBrd()
{
	short counter;

	for (auto r = 0; r <= NRANKSEXT; r++)
		for(auto f = 0; f <= NFILESEXT; f++)
		{
			if (r <= 1 || r >= NRANKSEXT-2 ||
			    f == 0 || f == NFILESEXT-1 )
			    Board::ExtBrd[r][f] = OFFBRD;

			else
			{
				Board::ExtBrd[r][f] = counter;
				counter++;
			}

		}
}


void Board::setBoardFromFEN_test()
{
	cout << "Starting FEN: "<< STARTFEN << endl;
	setBoardFromFEN(STARTFEN);
	printBoard();
	cout << "After WHITE move: " << TESTFEN1 << endl;
	setBoardFromFEN(TESTFEN1);
	printBoard();
	cout << "After BLACK move: " << TESTFEN2 << endl;
	setBoardFromFEN(TESTFEN2);
	printBoard();
	cout << "After WHITE move: " << TESTFEN3 << endl;
	setBoardFromFEN(TESTFEN3);
	printBoard(); 
}

void Board::initExtBrd_test()
{
	cout << "Printing Extended Board" << endl;

	for (auto r = NRANKSEXT-1; r >= 0; r--)
	{	
		for(auto f = 0; f <= NFILESEXT-1; f++)
		{
			cout << ExtBrd[r][f] << "\t";
		}
		
		cout << endl;
	}
}
