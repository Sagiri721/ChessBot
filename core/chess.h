#pragma once
#include "utils.h";

class Chess {

public:
	bool isSquareOccupied(std::vector<Utils::ChessPiece>, Vector2);
	std::vector<Utils::ChessPiece> effectuateMove(std::vector<Utils::ChessPiece>, Vector2, Vector2);
};