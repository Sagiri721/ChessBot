#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>

#include "chess.h"
#include "utils.h"

Utils c_utils = Utils();
bool Chess::turn = true;
std::vector<std::string> Chess::history;

// Start masks
bool Chess::occupationMask[8][8];
int Chess::colorMask[8][8];

bool Chess::isSquareOccupied(std::vector<Utils::ChessPiece> pieces, Vector2 square) {

	for (Utils::ChessPiece piece : pieces)
		if (piece.position.x == square.x && piece.position.y == square.y) return true;
	
	return false;
}

std::vector<Utils::ChessPiece> Chess::effectuateMove(std::vector<Utils::ChessPiece> pieces, Vector2 origin, Vector2 move) {

	if (!isSquareOccupied(pieces, origin)) return pieces;
	int currentPiece = c_utils.findPieceIndexFromPosition(pieces, origin);
	int targetPosition = c_utils.findPieceIndexFromPosition(pieces, move);

	if (!isMovelLegal(pieces, origin, move)) return pieces;
	if (pieces.at(currentPiece).color == turn) return pieces;

	if (targetPosition != -1)
		if (pieces.at(currentPiece).color == pieces.at(targetPosition).color) return pieces;
		else {
			pieces.erase(pieces.begin() + targetPosition);
			currentPiece = c_utils.findPieceIndexFromPosition(pieces, origin);
		}

	// Update move history
	c_utils.updateMoveHistory(pieces, origin, move);

	//Update move
	//Check if castling
	if ((pieces.at(currentPiece).index == 0 || pieces.at(currentPiece).index == 6) && abs(int(move.x) - int(origin.x)) == 2) {

		std::cout << "castlin";

		// Update counterpart rook position
		int xx = int(move.x) == 2 ? 0 : 7;
		int rookIndex = c_utils.findPieceIndexFromPosition(pieces, Vector2{ float(xx), origin.y });

		pieces.at(rookIndex).position = Vector2{ move.x + (int(move.x) == 2 ? 1 : -1), move.y};
	}
	pieces.at(currentPiece).position = move;
	if(!pieces.at(currentPiece).moved)	pieces.at(currentPiece).moved = true;

	c_utils.updateOccupationMask(pieces);

	turn = !turn;
	return pieces;
}

bool Chess::isMovelLegal(std::vector<Utils::ChessPiece> pieces, Vector2 origin, Vector2 move) {

	std::vector<Vector2> legalMoves = c_utils.getAllLegalPieceMoves(pieces, origin);
	for (Vector2 pos : legalMoves) if (pos.x == move.x && pos.y == move.y) return true;
	
	return false;
}

Utils::ChessPiece* Chess::isInCheck(std::vector<Utils::ChessPiece> pieces) {

	std::vector<Utils::ChessPiece> w_kings = c_utils.findPiecesFromPieceIndex(pieces, 0);
	std::vector<Utils::ChessPiece> b_kings = c_utils.findPiecesFromPieceIndex(pieces, 6);

	w_kings.insert(w_kings.end(), b_kings.begin(), b_kings.end());
	for (Utils::ChessPiece king : w_kings) {

		// Check if king is in any capture mask
	}

	return NULL;
}