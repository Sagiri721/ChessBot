#pragma once
#include "utils.h";

class Chess {

public:

	static bool turn, wCheck, bCheck;
	static std::string end;

	static bool occupationMask[8][8];
	static int colorMask[8][8];

	static std::vector<std::string> history;

	bool isSquareOccupied(std::vector<Utils::ChessPiece>, Vector2);
	std::vector<Utils::ChessPiece> effectuateMove(std::vector<Utils::ChessPiece>, Vector2, Vector2);
	bool isMovelLegal(std::vector<Utils::ChessPiece>, Vector2, Vector2);

	bool isInCheck(std::vector<Utils::ChessPiece>, bool);
	bool isSquareAttacked(std::vector<Utils::ChessPiece>, Vector2, bool);

	static void checkForMates(std::vector<Utils::ChessPiece>);
};