#include <iostream>
#include <string>

#include "board.h"

using std::cout;
using std::endl;
using std::string;

void Board::printBoard() const
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
			cout << val2char[Brd[r][f]] << "\t";
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

	}

	setPlistFromBrd();
}

void Board::setPlistFromBrd()
{
	for(int r = RANK8; r >= RANK1; r--)
		for(int f = FILEA; f <= FILEH; f++)
		{
			Square sq(r, f);
			int piece = getSquareValue(sq);

			if(piece)
				Plist[piece].push_back(sq);		
		}
}

void Board::resetBoard()
{
	for(int r = RANK8; r >= RANK1; r--)
		for(int f = FILEA; f <= FILEH; f++)
			Brd[r][f] = 0;

	for(int p = bP; p < NPIECES; p++)
		Plist[p].clear();
}


void Board::checkBoardConsistency() const
{
	// Check if every element of Plist is on Brd
	for(int piece = bP; piece < NPIECES; piece++)
		for(auto sq = Plist[piece].cbegin(); sq != Plist[piece].end(); sq++)
			if(getSquareValue(*sq) != piece)
			{
				sq->printSquare();
				cout << "Board inconsistent"<< " has no piece " << val2char[piece] << endl;
			}
	// Check if every Brd element is in Plist
	for(int r = RANK8; r >= RANK1; r--)
		for(int f = FILEA; f <= FILEH; f++)
		{
			Square sq(r, f);
			int piece = getSquareValue(sq);

			bool flag =false;
			if(piece)
			{
				for(auto sqp = Plist[piece].cbegin(); sqp != Plist[piece].end(); sqp++)
				{
					if(sq.isEqual(*sqp))
						flag = true;
				}
			}

			if(!flag && piece)
			{
				cout << "Board inconsistent" << val2char[piece] 
					 << " not in Piecelist" << r << "," << f << endl; 
			}
		}	
}

void Board::setBoardFromFEN_test()
{
	cout << "Starting FEN: "<< STARTFEN << endl;
	setBoardFromFEN(STARTFEN);
	printBoard();
	checkBoardConsistency();
	resetBoard();
	cout << "After WHITE move: " << TESTFEN1 << endl;
	setBoardFromFEN(TESTFEN1);
	printBoard();
	checkBoardConsistency();
	resetBoard();
	cout << "After BLACK move: " << TESTFEN2 << endl;
	setBoardFromFEN(TESTFEN2);
	printBoard();
	checkBoardConsistency();
	resetBoard();
	cout << "After WHITE move: " << TESTFEN3 << endl;
	setBoardFromFEN(TESTFEN3);
	printBoard();
	checkBoardConsistency();
	resetBoard(); 
}


// side has the color which attcks the square
bool Board::isSquareAttacked(const Square &sq, const Color side) const
{
	for(int piece = bP; piece <= wK; piece = piece+1)
	{
		// Skip pieces of defender color
		if(PieceSide[piece] != side)
			continue;

		for(int dir_idx = 0; dir_idx < NAttackdir[piece]; dir_idx++)
		{
			Square sqp = sq;
			do
			{
				sqp.moveSquareOpposite((int *)AttackDir[piece][dir_idx]);
				
				if (isOffBoard(sqp))
					break;

				if (isPieceOccupiedSquare(sqp, piece))
				return true;

				if (isOccupiedSquare(sqp))
					break;

			}while(!isOffBoard(sqp) && Ranged[piece]);
				// Non-ranged piece run this once
				// Ranged piece run until square is offboard
		}// end of attack direction for loop	
	}//end of piece loop
	return false;
}

int Board::getSquareValue(const Square &sq) const
{
	return Brd[sq.Pos[0]][sq.Pos[1]];
}

bool Board::isPieceOccupiedSquare(const Square &sq, const int p) const
{
	if (getSquareValue(sq) == p)
		return true;
	return false;
}

bool Board::isOccupiedSquare(const Square &sq) const
{
	if (getSquareValue(sq))
		return true;
	return false;
}

bool Board::isOffBoard(const Square &sq) const
{
	if (sq.Pos[0] < 0 || sq.Pos[0] > 7 || sq.Pos[1]  < 0 || sq.Pos[1] > 7)
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
		"8/1R3P2/1R2p3/8/8/4r3/8/8 w - - 0 1 ",
		"8/2Q5/3P4/8/8/8/5q2/8 w - - 0 1 "

	};
	const int num_test = 6;

	for(int test = 0; test < num_test; test++)
	{
		setBoardFromFEN(testFEN[test]);
		printBoard();

		cout << "Squares attacked by WHITE" << endl;
		for(int r = RANK8; r >= RANK1; r--)
		{
			cout << "RANK " << r+1 << "\t";
			
			for(int f = FILEA; f <= FILEH; f++)
			{
				Square sq(r, f);
				if (isSquareAttacked(sq, WHITE) && !isOccupiedSquare(sq))
					cout << "x" << "\t";
				else
					cout << "_" << "\t";
			}
			
			cout << endl << endl;
		}

		cout << "Square attacked by BLACK:" << endl;
		for(int r = RANK8; r >= RANK1; r--)
		{
			cout << "RANK " << r+1 << "\t";
			
			for(int f = FILEA; f <= FILEH; f++)
			{
				Square sq(r, f);
				if (isSquareAttacked(sq, BLACK) && !isOccupiedSquare(sq))
					cout << "x" << "\t";
				else
					cout << "_" << "\t";
			}
			
			cout << endl << endl;
		}


	}
}