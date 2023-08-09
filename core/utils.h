#pragma once
#include <string>
#include <vector>
#include <map>

struct Settings {

	int colour = 1;
	bool random = false;
	bool flipped = false;

	unsigned int theme = 0, pieces = 0;

	//Time controll
	int rawTime = 10, moveIncrement = 0;
};

const static std::map<int, int> pieceValueMap = {

	{0, 0}, {6, 0},
	{1, 9}, {7, 9},
	{2, 3}, {8, 3},
	{3, 3}, {9, 3},
	{4, 5}, {10, 5},
	{5, 1}, {11, 1},
};

class Utils {

	public:

		const std::string startPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
		static Settings appSettings;

		struct ChessPiece {

			unsigned int index;
			bool color;
			bool moved = false;
			bool enpassantable = false;

			Vector2 position;

			unsigned short captureMask = 0;
		};

		static void loadSettings();

		std::vector<ChessPiece> parseFenStringToBoardInformation(std::string);
		std::vector<Vector2> parseStringNotationToMove(std::string);
		std::string parseMoveToStringNotation(std::vector<Utils::ChessPiece>, Vector2, Vector2);

		int findPieceIndexFromPosition(std::vector<Utils::ChessPiece>, Vector2);
		
		ChessPiece* findPieceFromPosition(std::vector<Utils::ChessPiece>, Vector2);

		ChessPiece* findFirstPieceFromPieceIndex(std::vector<Utils::ChessPiece>, int);
		std::vector<ChessPiece> findPiecesFromPieceIndex(std::vector<Utils::ChessPiece>, int);

		std::vector<Vector2> getAllLegalPieceMoves(std::vector<Utils::ChessPiece>, Vector2, bool, bool);

		void updateOccupationMask(std::vector<Utils::ChessPiece>);
		void updateMoveHistory(std::vector<Utils::ChessPiece>, Vector2, Vector2);
};