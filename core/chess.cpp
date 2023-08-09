#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>
#include <tuple>;

#include "chess.h"
#include "utils.h"

Utils c_utils = Utils();
std::vector<std::string> Chess::history;

bool Chess::turn = true;
bool Chess::wCheck = false;
bool Chess::bCheck = false;
std::string Chess::end = "";
int Chess::promoteTo = 1;

float Chess::whiteTime = 0, Chess::blackTime = 0;

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

	if ((pieces.at(currentPiece).index == 5 && move.y == 0) || (pieces.at(currentPiece).index == 11 && move.y == 7)) {

		//Promote
		if (!pieces.at(currentPiece).color) pieces.at(currentPiece).index = promoteTo;
		else pieces.at(currentPiece).index = promoteTo + 6;
	}

	// Update move history
	c_utils.updateMoveHistory(pieces, origin, move);

	// Remove en passants
	int index = 0;
	for (Utils::ChessPiece p : pieces) {
		if ((p.index == 5 || p.index == 11) && p.enpassantable) pieces.at(index).enpassantable = false;
		index++;
	}

	int distance = abs(move.y - pieces.at(currentPiece).position.y);
	pieces.at(currentPiece).position = move;
	if (!pieces.at(currentPiece).moved) { 

		pieces.at(currentPiece).moved = true;
		if ((pieces.at(currentPiece).index == 5 || pieces.at(currentPiece).index == 11) && distance == 2)
			pieces.at(currentPiece).enpassantable = true;
	}

	c_utils.updateOccupationMask(pieces);

	// Check if check
	Utils::ChessPiece w_king = c_utils.findPiecesFromPieceIndex(pieces, 0)[0];
	Utils::ChessPiece b_king = c_utils.findPiecesFromPieceIndex(pieces, 6)[0];

	Chess::wCheck = isInCheck(pieces, !w_king.color);
	Chess::bCheck = isInCheck(pieces, !b_king.color);

	// Check for checkmate
	checkForMates(pieces);

	// Add clock time
	if (Chess::turn) Chess::whiteTime += Utils::appSettings.moveIncrement;
	else Chess::blackTime+= Utils::appSettings.moveIncrement;

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

void Chess::checkForMates(std::vector<Utils::ChessPiece> pieces) {

	int whiteMoveCount = 0, blackMoveCount = 0;
	bool checking_color = true;
	for (int i = 0; i < 2; i++) {

		for (Utils::ChessPiece piece : pieces) {

			if (piece.color == checking_color) {

				std::vector<Vector2> legalMoves = c_utils.getAllLegalPieceMoves(pieces, piece.position, false, true);
				for (Vector2 move : legalMoves) {

					if (Chess::colorMask[int(move.x)][int(move.y)] == int(checking_color)) continue;

					std::tuple<Vector2, Vector2> play = std::make_tuple(piece.position, move);
					if (checking_color) blackMoveCount++; else whiteMoveCount++;
				}
			}
		}

		checking_color = false;
	}

	if (Utils::appSettings.colour) {

		if (whiteMoveCount == 0 && Chess::wCheck)
			Chess::end = "White was checkmated";
		else if (whiteMoveCount == 0) 
			Chess::end = "White was stalemated";
	}
	else {

		if (blackMoveCount == 0 && Chess::bCheck)
			Chess::end = "Black was checkmated";
		else if (blackMoveCount == 0)
			Chess::end = "Black was stalemated";
	}
}