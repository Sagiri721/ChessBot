#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>

#include "chess.h"
#include "utils.h"

Utils c_utils = Utils();
bool Chess::turn = true;

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