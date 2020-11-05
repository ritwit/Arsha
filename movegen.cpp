#include <iostream>

#include "board.h"
#include "movegen.h"
#include "square.h"
#include "debug.h"

using std::cout;
using std::endl;


void Move::applyMove(Board &bd) const
{
	int capture_piece = bd.getSquareValue(To);
	int piece = bd.getSquareValue(From);
	
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
		
		// If bd already has an enp squre that is not empty, remove it 
		if( !OFFSQ.isEqual(bd.getEnpSquare()) && !PawnJump )
			bd.setEnpSquare(OFFSQ);

		ASSERT(bd.checkBoardConsistency());
		return;
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


void MoveGenerator::generateMoves(const Board &bd)
{
	addCastleMoves(bd);

	for(int piece = bP; piece < NPIECES; piece++)
		for (Square sq: bd.Plist[piece])
		{
			// If it's opponents piece, skip
			if(PieceSide[piece] != bd.ActiveColor)
				continue;

			for(int dir_idx = 0; dir_idx < NMovedir[piece]; dir_idx++)
			{
				Square capture_sq = sq;
				do
				{
					capture_sq.moveSquare((int *)MoveDir[piece][dir_idx]);
					int capture_piece = bd.getSquareValue(capture_sq);
					
					// Occupied by same color
					if (PieceSide[capture_piece] == bd.ActiveColor)
						break;

					// Pawns cannot capture in the direction they move
					// They can do two square jump, capture by En Passant
					// and get promoted. So, they are kinda special
					// The guy who invented chess didn't want to
					// make it easy for programmers obviously.
					else if (piece == wP || piece == bP)
					{
						addPawnMoves(bd, sq, piece);
						break;
					}
					
					// If empty square, add move to list
					else if (capture_piece == NO_PIECE)
					{
						MoveList.push_back(Move(sq, capture_sq, piece));
						continue;
					}

					// Occupied by opposite color, add capture
					else if (PieceSide[capture_piece] != bd.ActiveColor)
					{	
						MoveList.push_back(Move(sq, capture_sq, piece, capture_piece));
						break;
					}

				}while(!bd.isOffBoard(capture_sq) && Ranged[piece]);
				// Unranged loop once
			} // Move direction loop 
		} // Piece list loop
	removeIllegalMoves();
}

void MoveGenerator::addCastleMoves(const Board &bd)
{

}



// King of active color cannot be in check
// King of opposite color cannot be captured
void MoveGenerator::removeIllegalMoves()
{
	//MoveList.push_back(mv);
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
	/*
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
	*/
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
		mv.printMove();
		print_separator();
		Board bdt = bd;
		mv.applyMove(bdt);
		bdt.printBoard();
		print_double_separator();
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
