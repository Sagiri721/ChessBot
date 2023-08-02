#pragma once
#include <string>
#include <vector>

class Utils {

	public:

		const std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
		struct ChessPiece {

			int index;
			bool color;

			Vector2 position;
		};

		std::vector<ChessPiece> parseFenStringToBoardInformation(std::string);
		std::vector<Vector2> parseStringNotationToMove(std::string);

		int findPieceIndexFromPosition(std::vector<Utils::ChessPiece>, Vector2);
};