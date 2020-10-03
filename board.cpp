#include <iostream>
#include <string>

#include "board.h"

using std::cout;
using std::endl;
using std::string;

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
				case '-':
					break;
				default:
					cout << "ERROR: invalid castle" << endl;
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


// side has the color which attcks the square
bool Board::isSquareAttacked(const int r, const int f, const Color side)
{
	for(int p = bP; p <= wK; p=p+1)
	{
		// Skip pieces of defender color
		if(PieceSide[p] != side)
			continue;

		#ifdef DEBUG
		if (r == RANK6 && f == FILEB)
		cout << "Checking attacks by 	" << val2char[p] << endl;
		#endif
   		
   		// If the piece is not ranged
		if(!Ranged[p])
		{
			for (int dir_idx = 0; dir_idx < NAttackdir[p]; dir_idx++)
			{
				int rp = r - AttackDir[p][dir_idx][0];
				int fp = f - AttackDir[p][dir_idx][1];

				if (isOffBoard(rp, fp))
					continue;

				if (isPieceOccupiedSquare(rp, fp, p))
				{
					return true;
				}
			}

		}

		// If it's a ranged unit
		else
		{
			for (int dir_idx = 0; dir_idx < NAttackdir[p]; dir_idx++)
			{
				int rp = r;
				int fp = f;
				int range = 1;

				#ifdef DEBUG
				//if (r == RANK6 && f == FILEB)
				//	cout << r << f << endl;
				#endif

				while(!isOffBoard(rp, fp))
				{
					rp = r - AttackDir[p][dir_idx][0]*range;
					fp = f - AttackDir[p][dir_idx][1]*range;
					range++;
					
					#ifdef DEBUG
					//if (r == RANK6 && f == FILEB)
					cout << rp<< fp <<" " << range<< " "<< AttackDir[p][dir_idx][0] << " " << AttackDir[p][dir_idx][1] << endl;
					#endif

					if (isOffBoard(rp, fp))
						break;

					if (isPieceOccupiedSquare(rp, fp, p))
						return true;

					if (isOccupiedSquare(rp, fp))
						break;

				}
			}
		} 
		
	}

	return false;
}

bool Board::isPieceOccupiedSquare(const int r, const int f, const int p)
{
	if (Brd[r][f] == p)
		return true;
	return false;
}

bool Board::isOccupiedSquare(const int r, const int f)
{
	if (Brd[r][f])
		return true;
	return false;
}

bool Board::isOffBoard(const int r, const int f)
{
	if (r < 0 || r > 7 || f  < 0 || f > 7)
		return true;
	return false; 
}

void Board::isSquareAttacked_test()
{
	const string testFEN[] =
	{
		"8/1N6/8/8/4n3/8/8/8 w - - 0 1 ",		
		"8/3p4/4pp2/8/8/2PP4/4PP2/8 w - - 0 1 ",
		"8/2K5/8/8/8/5k2/8/8 w - - 0 1 ",
		"8/2B2B2/8/8/3P2P1/4b3/4b3/8 w - - 0 1 ",
		"8/1R6/1R6/8/1P6/5r2/8/8 w - - 0 1 ",
		"8/2Q5/3P4/8/8/8/5q2/8 w - - 0 1 "

	};
	const int num_test = 6;

	for(int test = 0; test < num_test; test++)
	{
		setBoardFromFEN(testFEN[test]);
		printBoard();

		cout << "WHITE attacks:" << endl;
		for(int r = RANK8; r >= RANK1; r--)
		{
			cout << "RANK " << r+1 << "\t";
			
			for(int f = FILEA; f <= FILEH; f++)
			{
				if (isSquareAttacked(r, f, WHITE) && !isOccupiedSquare(r, f))
					cout << "x" << "\t";
				else
					cout << "_" << "\t";
			}
			
			cout << endl << endl;
		}

		cout << "BLACK attacks:" << endl;
		for(int r = RANK8; r >= RANK1; r--)
		{
			cout << "RANK " << r+1 << "\t";
			
			for(int f = FILEA; f <= FILEH; f++)
			{
				if (isSquareAttacked(r, f, BLACK) && !isOccupiedSquare(r, f))
					cout << "x" << "\t";
				else
					cout << "_" << "\t";
			}
			
			cout << endl << endl;
		}


	}
}