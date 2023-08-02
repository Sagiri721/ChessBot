#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>

#include "core/utils.h"
#include "core/chess.h"

const int boardSize = 8;
const int tileSize = 70;

const Color 
	whiteColor = Color{ 118, 150, 86, 255 }, 
	blackColor = Color{ 238, 238, 210, 255 },
	highlightColor = RED;

Texture2D pieceSprite;
Rectangle pieces[12];

// Util reference
Utils utils = Utils();
Chess chess= Chess();

// Current pieces
std::vector<Utils::ChessPiece> myPieces;

Vector2 highlight = Vector2{-1, -1};
void checkForClicks() {

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

		Vector2 mouse = GetMousePosition();
		Vector2 savedHighlight = highlight;

		if (highlight.x != -1) {

			Vector2 target = Vector2{
				floor(mouse.x / tileSize),
				floor(mouse.y / tileSize),
			};

			// Move
			std::cout << highlight.x;
			myPieces = chess.effectuateMove(myPieces, highlight, target);

			highlight = Vector2{ -1, -1 };
			return;
		}

		highlight = Vector2{
			floor(mouse.x / tileSize),
			floor(mouse.y / tileSize),
		};

		if (!chess.isSquareOccupied(myPieces, highlight)) highlight = savedHighlight;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) highlight = Vector2{ -1, -1 };
}

void chessboard() {

	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++) {

			Color col = (i + j) % 2 == 0 ? whiteColor : blackColor;
			if (i == int(highlight.x) && j == int(highlight.y)) col = highlightColor;

			DrawRectangle(
				i * tileSize, 
				j * tileSize, 
				tileSize, 
				tileSize,
				col
			);
		}

	for (Utils::ChessPiece piece : myPieces) {

		//std::cout << piece.position.x << std::endl;
		DrawTexturePro(pieceSprite, pieces[piece.index], Rectangle{tileSize * piece.position.x, tileSize * piece.position.y, tileSize, tileSize }, Vector2{0, 0}, 0, WHITE);
	}
}

void setupPieces() {

	const float spriteSize = 426.5;

	pieceSprite = LoadTexture("resources/pieces.png");
	for (int i = 0; i < 12; i++)
	{
		int isBlack = (i >= 6 ? 1 : 0);
		pieces[i] = Rectangle{
			i * spriteSize - (isBlack * spriteSize * 6),
			isBlack * spriteSize,
			spriteSize,
			spriteSize
		};
	};

	/*
	0 - w king 6 black
	1 - w queen 7 black
	2 - w bishop 8 black
	3 - w knight 9 black
	4 - w rook 10 black
	5 - w pawn 11 black
	*/
}

void placePieces(std::string fenString) {

	std::vector<Utils::ChessPiece> curPieces = utils.parseFenStringToBoardInformation(fenString);
	
	myPieces.clear();
	for (Utils::ChessPiece p : curPieces) myPieces.push_back(p);
}

int main() {

	const int screenWidth = 830;
	const int screenHeight = 560;

	// Start window
	InitWindow(screenWidth, screenHeight, "Chess board interface");
	SetTargetFPS(60);

	setupPieces();
	placePieces(utils.startPosition);

	// Main loop
	while (!WindowShouldClose()) {

		//Update
		checkForClicks();

		BeginDrawing();
		
		ClearBackground(BLACK);
		//Draw board
		chessboard();

		DrawText("Chessboard interface", tileSize * boardSize + 5, 10, 20, LIGHTGRAY);

		EndDrawing();
	}

	// Flush data
	CloseWindow();

	return 0;
}