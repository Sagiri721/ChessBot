#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>

#include "chess.h"
#include "utils.h"
#include <tuple>;

Utils c_utils = Utils();
std::vector<std::string> Chess::history;

bool Chess::turn = true;
bool Chess::wCheck = false;
bool Chess::bCheck = false;
std::string Chess::end = "";

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

	if (pieces.at(currentPiece).index == 0 || pieces.at(currentPiece).index == 6) {

		//Check if castling
		if (abs(int(move.x) - int(origin.x)) == 2) {

			// Update counterpart rook position
			int xx = int(move.x) == 2 ? 0 : 7;
			int rookIndex = c_utils.findPieceIndexFromPosition(pieces, Vector2{ float(xx), origin.y });

			pieces.at(rookIndex).position = Vector2{ move.x + (int(move.x) == 2 ? 1 : -1), move.y };
		}
	}

	// Update move history
	c_utils.updateMoveHistory(pieces, origin, move);

	pieces.at(currentPiece).position = move;
	if(!pieces.at(currentPiece).moved)	pieces.at(currentPiece).moved = true;

	c_utils.updateOccupationMask(pieces);

	// Check if check
	Utils::ChessPiece w_king = c_utils.findPiecesFromPieceIndex(pieces, 0)[0];
	Utils::ChessPiece b_king = c_utils.findPiecesFromPieceIndex(pieces, 6)[0];

	Chess::wCheck = isInCheck(pieces, !w_king.color);
	Chess::bCheck = isInCheck(pieces, !b_king.color);

	// Check for checkmate
	checkForMates(pieces);

	turn = !turn;
	return pieces;
}

bool Chess::isMovelLegal(std::vector<Utils::ChessPiece> pieces, Vector2 origin, Vector2 move) {

	std::vector<Vector2> legalMoves = c_utils.getAllLegalPieceMoves(pieces, origin, false, false);
	for (Vector2 pos : legalMoves) if (pos.x == move.x && pos.y == move.y) return true;
	
	return false;
}

bool Chess::isInCheck(std::vector<Utils::ChessPiece> pieces, bool color) {

	std::vector<Utils::ChessPiece> king = c_utils.findPiecesFromPieceIndex(pieces, color ? 0 : 6);

	bool checked = isSquareAttacked(pieces, king[0].position, !king[0].color);
	//std::cout << "king: " << king[0].position.x << " " << king[0].position.y << ", " << checked << "\n";

	return checked;
}

bool Chess::isSquareAttacked(std::vector<Utils::ChessPiece> pieces, Vector2 square, bool color) {

	for (Utils::ChessPiece piece : pieces) {

		if (piece.color == color) {

			std::vector<Vector2> moves = c_utils.getAllLegalPieceMoves(pieces, piece.position, true, false);
			//std::cout << "Move length " << moves.size()<< "\n";
			for (Vector2 move : moves) { 
				//std::cout << "move: " << move.x << ", " << move.y << "\n";
				if (int(move.x) == square.x && int(move.y) == square.y) return true;}
		}
	}

	return false;
}