#pragma once
#include <string>
#include <vector>

class Utils {

	public:

		const std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN2KBNR";

		struct ChessPiece {

			unsigned int index;
			bool color;
			bool moved = false;

			Vector2 position;

			unsigned short captureMask = 0;
		};

		std::vector<ChessPiece> parseFenStringToBoardInformation(std::string);
		std::vector<Vector2> parseStringNotationToMove(std::string);
		std::string parseMoveToStringNotation(std::vector<Utils::ChessPiece>, Vector2, Vector2);

		int findPieceIndexFromPosition(std::vector<Utils::ChessPiece>, Vector2);
		
		ChessPiece* findPieceFromPosition(std::vector<Utils::ChessPiece>, Vector2);

		ChessPiece* findFirstPieceFromPieceIndex(std::vector<Utils::ChessPiece>, int);
		std::vector<ChessPiece> findPiecesFromPieceIndex(std::vector<Utils::ChessPiece>, int);

		std::vector<Vector2> getAllLegalPieceMoves(std::vector<Utils::ChessPiece>, Vector2, bool);

		void updateOccupationMask(std::vector<Utils::ChessPiece>);
		void updateMoveHistory(std::vector<Utils::ChessPiece>, Vector2, Vector2);
};