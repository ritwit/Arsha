#include <iostream>
#include <algorithm>

#include "board.h"
#include "movegen.h"
#include "square.h"
#include "debug.h"

using std::cout;
using std::endl;
using std::string;


void Move::applyMove(Board &bd) const
{
	int capture_piece = bd.getSquareValue(To);
	int piece = bd.getSquareValue(From);

	// I like this goto! I dont care what you think
	// After the move, change activeplayer on board,
	// check castle perm etc
	if(Castle)
	{
		applyCastleMove(bd);
		goto finish;
	}

	ASSERT(!To.isOffSquare());
	ASSERT(!From.isOffSquare());

	// Change Brd
	bd.setBrdValue(To, piece);
	bd.setBrdValue(From, NO_PIECE);

	// Change Plist
	bd.movePiecePlist(piece, From, To);


	if(Enp.isEqual(OFFSQ))
	{
		ASSERT(capture_piece == Capture);
		ASSERT(piece == Piece);
	}

	// If there's an ENP capture
	if(!Enp.isEqual(OFFSQ))
	{
		// The square of the piece that will be captured
		Square enp_capture = getCaptureEnpSquare();
		// The piece to be captured
		int enp_capture_piece = bd.getSquareValue(enp_capture);
		// Remove that piece from brd and Plist
		bd.setBrdValue(enp_capture, NO_PIECE);
		bd.removePiecePlist(enp_capture_piece, enp_capture);
		goto finish;
	}

	// Ordinary capture
	else if(Capture)
		bd.removePiecePlist(capture_piece, To);

	// Capture and promotion are possible together
	if(Promotion)
	{
		bd.setBrdValue(To, Promotion);
		bd.removePiecePlist(piece, To);
		bd.addPiecePlist(Promotion, To);
		goto finish;
	}

	// The initial double square jump is done, create enp capture square
	if(PawnJump == PJUMP)
	{
		// The square available for enp capture
		Square enp = generateEnpSquare();
		bd.setEnpSquare(enp);
	}

	finish:
	{
		// Increment play counter and switch color
		Color opposite_color = bd.ActiveColor == WHITE ? BLACK : WHITE;
		bd.ActiveColor = opposite_color;

		// If rook or king moved, remove castle perm
		updateCastlePermissions(bd);

		// If bd already has an enp squre that is not empty, remove it
		// Enp can be only used in the immediate next move
		if( !OFFSQ.isEqual(bd.getEnpSquare()) && !PawnJump )
			bd.setEnpSquare(OFFSQ);

		ASSERT(bd.checkBoardConsistency());
		return;
	}
}

void Move::updateCastlePermissions(Board &bd) const
{
	if(bd.getSquareValue(Square(RANK1, FILEA)) != wR)
		bd.Castle[1] = 0;

	if(bd.getSquareValue(Square(RANK1, FILEH)) != wR)
		bd.Castle[0] = 0;

	if(bd.getSquareValue(Square(RANK1, FILEE)) != wK)
	{
		bd.Castle[0] = 0;
		bd.Castle[1] = 0;
	}

	if(bd.getSquareValue(Square(RANK8, FILEA)) != bR)
		bd.Castle[3] = 0;

	if(bd.getSquareValue(Square(RANK8, FILEH)) != bR)
		bd.Castle[2] = 0;

	if(bd.getSquareValue(Square(RANK8, FILEE)) != bK)
	{
		bd.Castle[2] = 0;
		bd.Castle[3] = 0;
	}
}

// The square of the piece that got captured by enp
Square Move::getCaptureEnpSquare() const
{
	// Rank of From Square and file of To Square
	int rank = From.Pos[0];
	int file = To.Pos[1];
	return Square(rank, file);
}

// The square that the pawn can move to capture via enp (same
// as the enp square in FEN)
Square Move::generateEnpSquare() const
{
	// File of To square and rank between To and From
	int rank = (To.Pos[0] + From.Pos[0]) / 2;
	int file = To.Pos[1];
	return Square(rank, file);
}

void Move::applyCastleMove(Board &bd) const
{
	int king_piece;
	int rook_piece;
	Square king_init;
	Square king_final;
	Square rook_init;
	Square rook_final;
	// Black king side
	if (Castle == BKINGSIDE)
	{
		king_piece = bK;
		rook_piece = bR;
		king_init.setSquare(RANK8, FILEE);
		king_final.setSquare(RANK8, FILEG);
		rook_init.setSquare(RANK8, FILEH);
		rook_final.setSquare(RANK8, FILEF);
	}
	// Black queen side
	else if (Castle == BQUEENSIDE)
	{
		king_piece = bK;
		rook_piece = bR;
		king_init.setSquare(RANK8, FILEE);
		king_final.setSquare(RANK8, FILEC);
		rook_init.setSquare(RANK8, FILEA);
		rook_final.setSquare(RANK8, FILED);
	}
	// White king side
	else if (Castle == WKINGSIDE)
	{
		king_piece = wK;
		rook_piece = wR;
		king_init.setSquare(RANK1, FILEE);
		king_final.setSquare(RANK1, FILEG);
		rook_init.setSquare(RANK1, FILEH);
		rook_final.setSquare(RANK1, FILEF);
	}
	// White queen side
	else if (Castle == WQUEENSIDE)
	{
		king_piece = wK;
		rook_piece = wR;
		king_init.setSquare(RANK1, FILEE);
		king_final.setSquare(RANK1, FILEC);
		rook_init.setSquare(RANK1, FILEA);
		rook_final.setSquare(RANK1, FILED);
	}

	// Shouldnt be here
	else
	{
		ASSERT(false);
	}

	// Change Brd
	bd.setBrdValue(king_final, king_piece);
	bd.setBrdValue(king_init, NO_PIECE);
	bd.setBrdValue(rook_final, rook_piece);
	bd.setBrdValue(rook_init, NO_PIECE);

	// Change Plist
	bd.movePiecePlist(king_piece, king_init, king_final);
	bd.movePiecePlist(rook_piece, rook_init, rook_final);

	ASSERT(bd.checkBoardConsistency());

}

void Move::printMove() const
{
	cout << "Printing Move" << endl;

	cout << "From: ";
	From.printSquare();
	cout << "To: ";
	To.printSquare();
	cout << "ENpass:";
	Enp.printSquare();

	cout << "Piece: " << val2char[Piece] << endl;
	cout << "Capture: " << Capture << endl;
	cout << "Promotion: " << Promotion << endl;
	cout << "Castle: " << Castle << endl;
	cout << "PawnJump: " << PawnJump <<endl;
}

string Move::getString() const
{
	if (Castle != NOCASTLE)
	{
		return castleside2string[Castle];
	}
	string from_str = From.getString();
	string to_str = To.getString();
	string promotion_str = "";
	if (Promotion != NO_PIECE)
		promotion_str = val2char[Promotion];
	return from_str + to_str + promotion_str;
}


void MoveGenerator::generateMoves(const Board &bd)
{
	addCastleMoves(bd);

	for(int piece = bP; piece < NPIECES; piece++)
	{
		INFO("Finding moves for " << val2char[piece]);
		for (Square sq: bd.Plist[piece])
		{
			// If it's opponents piece, skip
			if(PieceSide[piece] != bd.ActiveColor)
			{
				INFO("Not Active color; skip");
				continue;
			}

			// Pawns cannot capture in the direction they move
			// They can do two square jump, capture by En Passant
			// and get promoted. So, they are special.
			// The guy who invented chess didn't want to
			// make it easy for programmers obviously.
			else if (piece == wP || piece == bP)
			{
				addPawnMoves(bd, sq, piece);
				INFO("Added Pawn Moves");
				continue;
			}

			// Non-Pawn pieces
			for(int dir_idx = 0; dir_idx < NMovedir[piece]; dir_idx++)
			{
				Square capture_sq = sq;
				do
				{
					capture_sq.moveSquare((int *)MoveDir[piece][dir_idx]);
					if (bd.isOffBoard(capture_sq))
						break;
					int capture_piece = bd.getSquareValue(capture_sq);

					// Occupied by same color
					// Skips if square is empty
					if (PieceSide[capture_piece] == bd.ActiveColor)
						break;

					// If empty square, add move to list
					else if (capture_piece == NO_PIECE)
					{
						MoveList.push_back(Move(sq, capture_sq, piece));
						INFO("Adding Move to empty square");
						continue;
					}

					// Occupied by opposite color, add capture
					else if (PieceSide[capture_piece] != bd.ActiveColor)
					{
						MoveList.push_back(Move(sq, capture_sq, piece, capture_piece));
						INFO("Adding capture move");
						break;
					}

				}while(!bd.isOffBoard(capture_sq) && Ranged[piece]);
				// Unranged loop once
			} // Move direction loop
		} // Piece list loop
	}// Piece type loop
	removeIllegalMoves(bd);
}

void MoveGenerator::test_generateMoves(Board &bd)
{
	std::vector<string> test_fens;
	//test_fens.push_back(STARTFEN);
	//test_fens.push_back("rN2k2r/8/8/8/8/8/8/R3Kn1R w KQkq - 0 1 ");
	test_fens.push_back("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 ");

	for(string &fen: test_fens)
	{
		bd.resetBoard();
		clearMoves();
		bd.setBoardFromFEN(fen);
		bd.printBoard();
		print_separator();
		cout << "Generating all possible legal moves" << endl;
		generateMoves(bd);
		cout << "Number of Moves " << getNumMoves();
		//printAllMovesGenerated(bd);
		print_double_separator();
		cout << "Next case" << endl;
		print_double_separator();
	}
	cout << "End Castling test" << endl;
}


int MoveGenerator::getNumMoves() const
{
	return MoveList.size();
}

void MoveGenerator::removeIllegalMoves(const Board &bd)
{
	// lambda for erase-remove idiom to detect illegal moves
	auto illegal_move = [bd](const Move &mv)
	{
		Board bdt = bd;
		// After applying move, active color change
		Color opposite_color = bdt.ActiveColor == WHITE ? BLACK : WHITE;
		mv.applyMove(bdt);
		Square king_sq = opposite_color == BLACK ?
			bdt.Plist[wK][0] : bdt.Plist[bK][0];
		return bdt.isSquareAttacked(king_sq, opposite_color);
	};

	/*
	for (Move &mv: MoveList)
	{
		if (illegal_move(mv))
		{
			mv.printMove();
			cout << "illegal_move" << endl;
		}
		else
		{
			mv.printMove();
			cout << "legal move" << endl;
		}
		print_separator();
	}
	*/
	auto end = std::remove_if(MoveList.begin(), MoveList.end(), illegal_move);
	MoveList.erase(end, MoveList.end());
}

void MoveGenerator::addCastleMoves(const Board &bd)
{
	// It's ok if the king ends up in check.
	// Another function will remove these moves.

	// If no castle permissions return
	if (!(bd.Castle[0] || bd.Castle[1] || bd.Castle[2] || bd.Castle[3]))
		return;

	int king_side;
	int queen_side;
	Square king_side_passage;
	Square queen_side_passage;
	Square queen_side_rook_passage;
	Square king_final_kside;
	Square king_final_qside;
	Color opposite_color;

	Square king_sq = bd.ActiveColor == WHITE ?
					 bd.Plist[wK][0] : bd.Plist[bK][0];

	if (bd.ActiveColor == WHITE)
	{
		king_side = 0;
		queen_side = 1;
		opposite_color = BLACK;
		king_side_passage.setSquare(RANK1, FILEF);
		queen_side_passage.setSquare(RANK1, FILED);
		king_final_kside.setSquare(RANK1, FILEG);
		king_final_qside.setSquare(RANK1, FILEC);
		queen_side_rook_passage.setSquare(RANK1, FILEB);
	}
	else
	{
		king_side = 2;
		queen_side = 3;
		opposite_color = WHITE;
		king_side_passage.setSquare(RANK8, FILEF);
		queen_side_passage.setSquare(RANK8, FILED);
		king_final_kside.setSquare(RANK8, FILEG);
		king_final_qside.setSquare(RANK8, FILEC);
		queen_side_rook_passage.setSquare(RANK8, FILEB);
	}

	// If king attacked, return
	if(bd.isSquareAttacked(king_sq, opposite_color))
		return;

	// Push Castle king side if permissible
	// Check if squares are empty or attacked
	if(bd.Castle[king_side] &&
	   !bd.isSquareAttacked(king_side_passage, opposite_color) &&
	   !bd.isOccupiedSquare(king_side_passage) &&
	   !bd.isOccupiedSquare(king_final_kside)
	   	)
	{
		int castle_side = bd.ActiveColor == WHITE ?
			WKINGSIDE : BKINGSIDE;
		MoveList.push_back(Move(OFFSQ, OFFSQ, NO_PIECE,
			NO_PIECE, OFFSQ, NO_PIECE, castle_side));
		INFO("Added king side castle move");
		INFO("Castleside = " << castle_side);
	}

	// Push Castle Queen side moves
	if(bd.Castle[queen_side] &&
	   !bd.isSquareAttacked(queen_side_passage, opposite_color) &&
	   !bd.isOccupiedSquare(queen_side_passage) &&
	   !bd.isOccupiedSquare(king_final_qside) &&
	   !bd.isOccupiedSquare(queen_side_rook_passage)
	   )
	{
		int castle_side = bd.ActiveColor == WHITE ?
			WQUEENSIDE : BQUEENSIDE;
		MoveList.push_back(Move(OFFSQ, OFFSQ, NO_PIECE,
			NO_PIECE, OFFSQ, NO_PIECE, castle_side));
		INFO("Added queen side castle move");
		INFO("Castleside = " << castle_side);
	}

}

void MoveGenerator::test_addCastleMoves(Board &bd)
{
	std::vector<string> test_fens;
	test_fens.push_back(STARTFEN);
	test_fens.push_back("rN2k2r/8/8/8/8/8/8/R3Kn1R w KQkq - 0 1 ");
	test_fens.push_back("rN2k2r/8/8/8/8/8/8/R3Kn1R b KQkq - 0 1 ");

	for(string &fen: test_fens)
	{
		bd.resetBoard();
		clearMoves();
		bd.setBoardFromFEN(fen);
		bd.printBoard();
		print_separator();
		cout << "Genrate Castle moves" << endl;
		addCastleMoves(bd);
		printAllMovesGenerated(bd);
		print_double_separator();
		cout << "Next test" << endl;
	}
	cout << "End Castling test" << endl;
}

void MoveGenerator::clearMoves()
{
	MoveList.clear();
}


void MoveGenerator::addPawnMoves(const Board &bd, const Square &sq, const int &piece)
{
	// Add double pawn double jump if squares infront are empty
	int pawn_start_rank = (PieceSide[piece]) == WHITE ? RANK2 : RANK7;
	Square single_jump = sq;
	single_jump.moveSquare((int *)MoveDir[piece][0]);
	Square double_jump = single_jump;
	double_jump.moveSquare((int *)MoveDir[piece][0]);

	// Add Normal Pawn move and promotion
	if (bd.getSquareValue(single_jump) == NO_PIECE)
	{
		// Normal
		if (!isPromotion(piece, single_jump))
			MoveList.push_back(Move(sq, single_jump, piece));

		// Promotion
		else
			addPromotionMoves(sq, single_jump, piece, NO_PIECE);
	}

	// Pawn double jump
	if (sq.Pos[0] == pawn_start_rank &&
	    bd.getSquareValue(single_jump) == NO_PIECE &&
	    bd.getSquareValue(double_jump) == NO_PIECE
	    )
	{
		MoveList.push_back(Move(sq, double_jump, piece, NO_PIECE,
		 OFFSQ, NO_PIECE, NOCASTLE, PJUMP));
	}

	// Add Capture moves
	// Really only two times looped, loop unroll?
	for(int dir_idx = 0; dir_idx < NAttackDir[piece]; dir_idx++)
	{
		// Get the square attacked and the piece placed there
		Square sq_capture = sq;
		sq_capture.moveSquare((int *)AttackDir[piece][dir_idx]);
		if(sq_capture.isOffBoard())
			continue;
		int piece_capture = bd.getSquareValue(sq_capture);

		// If there's something of opponents color on there, add capture
		if (piece_capture != NO_PIECE && PieceSide[piece_capture] != bd.ActiveColor)
		{
			if(!isPromotion(piece, sq_capture))
				MoveList.push_back(Move(sq, sq_capture, piece, piece_capture));
			else
				addPromotionMoves(sq, sq_capture, piece, piece_capture);
		}

		// Add Enp move
		else if (sq_capture.isEqual(bd.EnP))
		{
			MoveList.push_back(Move(sq, sq_capture, piece, NO_PIECE, sq_capture));
		}
	}
}

void MoveGenerator::test_addPawnMoves(Board &bd)
{

	bd.setBoardFromFEN(STARTFEN);
	bd.printBoard();
	print_separator();
	cout << "Genrate pawn moves on E file" << endl;
	addPawnMoves(bd, Square(RANK2, FILEE)  ,wP);
	printAllMovesGenerated(bd);

	// Promotion test
	bd.resetBoard();
	bd.setBoardFromFEN("K3B1n1/1Pp2P2/8/8/8/8/3p4/k7 w - - 0 1 ");
	bd.printBoard();
	print_separator();
	cout << "Promotion move generation testing" << endl;
	addPawnMoves(bd, Square(RANK7, FILEF)  ,wP);
	printAllMovesGenerated(bd);

	// Enp test
	bd.resetBoard();
	bd.setBoardFromFEN("k7/4ppp1/8/8/5Pp1/8/1PPPP3/K7 b - f3 0 1 ");
	bd.printBoard();
	print_separator();
	cout << "En Passant move generation testing" << endl;
	addPawnMoves(bd, Square(RANK4, FILEG) , bP);
	printAllMovesGenerated(bd);


}



void MoveGenerator::printAllMovesGenerated(Board &bd)
{
	for (Move &mv : MoveList)
	{
		//mv.printMove();
		//print_separator();
		Board bdt = bd;
		mv.applyMove(bdt);
		//bdt.printBoard();
		ASSERT(bdt.checkBoardConsistency());
		if(!bdt.checkBoardConsistency())
		{
			bdt.printBoard();
			cout << "Starting board" << endl;
			bd.printBoard();
			mv.printMove();
		}
		//print_double_separator();
	}

}

void MoveGenerator::addPromotionMoves(const Square &sq, const Square &single_jump,
	const int piece, const int capture)
{
	if(PieceSide[piece] == WHITE)
	{
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, wN));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, wB));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, wR));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, wQ));
	}
	else
	{
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, bN));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, bB));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, bR));
		MoveList.push_back(Move(sq, single_jump, piece, capture, OFFSQ, bQ));

	}
}

bool MoveGenerator::isPromotion(const int &piece, const Square &to)
{
	int promotion_rank = (PieceSide[piece] == WHITE) ? RANK8: RANK1;
	if (to.Pos[0] == promotion_rank)
		return true;
	else
		return false;
}

void Move::test_applyMove(Board &bd)
{
	// Should have used a loop !!
	cout << "Starting FEN: "<< STARTFEN << endl;
	bd.setBoardFromFEN(STARTFEN);
	bd.printBoard();
	print_separator();
	// Move white pawn
	cout << "Move white pawn E2 -> E3" << endl;
	*this = Move(Square(RANK2, FILEE), Square(RANK3, FILEE), wP);
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_double_separator();

	// Instead double pawn move which trigger pawnjump
	bd.resetBoard();
	bd.setBoardFromFEN(STARTFEN);
	cout << "Move white pawn E2 -> E4" << endl;
	*this = Move(Square(RANK2, FILEE), Square(RANK4, FILEE), wP,
		NO_PIECE, OFFSQ, NO_PIECE, NOCASTLE, 1);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_double_separator();

	// Test Castling
	std::string CastleFEN= "r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1 ";
	bd.resetBoard();
	bd.setBoardFromFEN(CastleFEN);
	bd.printBoard();
	cout << "White kingside Castle test" << endl;
	print_separator();
	*this = Move(OFFSQ, OFFSQ, NO_PIECE,
		NO_PIECE, OFFSQ, NO_PIECE, WKINGSIDE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_separator();

	cout << "White queenside Castle test" << endl;
	bd.resetBoard();
	bd.setBoardFromFEN(CastleFEN);
	print_separator();
	*this = Move(OFFSQ, OFFSQ, NO_PIECE,
		NO_PIECE, OFFSQ, NO_PIECE, WQUEENSIDE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_separator();

	cout << "Black kingside Castle test" << endl;
	bd.resetBoard();
	bd.setBoardFromFEN(CastleFEN);
	print_separator();
	*this = Move(OFFSQ, OFFSQ, NO_PIECE,
		NO_PIECE, OFFSQ, NO_PIECE, BKINGSIDE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_separator();

	cout << "Black queenside Castle test" << endl;
	bd.resetBoard();
	bd.setBoardFromFEN(CastleFEN);
	print_separator();
	*this = Move(OFFSQ, OFFSQ, NO_PIECE,
		NO_PIECE, OFFSQ, NO_PIECE, BQUEENSIDE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_separator();

	print_double_separator();
	cout << "Promotion test" << endl;
	std::string promotionFEN = "8/6P1/8/1k6/8/8/3K3p/8 w - - 0 1 ";
	bd.resetBoard();
	bd.setBoardFromFEN(promotionFEN);
	print_separator();
	*this = Move(Square(RANK7, FILEG), Square(RANK8, FILEG), wP,
		NO_PIECE, OFFSQ, wQ);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_double_separator();

	cout << "Capture tests" << endl;
	std::string captureFEN = "k4n2/1b4P1/8/8/3n4/5R2/3Q4/7K w - - 0 1 ";
	bd.resetBoard();
	bd.setBoardFromFEN(captureFEN);
	bd.printBoard();
	print_separator();
	*this = Move(Square(RANK7, FILEG), Square(RANK8, FILEF), wP,
		bN, OFFSQ, wQ, NOCASTLE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_separator();
	cout << "bN captures wR" << endl;
	bd.resetBoard();
	bd.setBoardFromFEN(captureFEN);
	bd.printBoard();
	print_separator();
	*this = Move(Square(RANK4, FILED), Square(RANK3, FILEF), bN,
		wR, OFFSQ, NO_PIECE, NOCASTLE, NOPJUMP);
	printMove();
	applyMove(bd);
	bd.printBoard();
	ASSERT(bd.checkBoardConsistency());
	print_double_separator();
}
