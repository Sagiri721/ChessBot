#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
#include <tuple>

#include "core/utils.h"
#include "core/chess.h"
#include "core/core.h"

const int boardSize = 8;
const int tileSize = 70;

const Color 
	whiteColor = Color{ 118, 150, 86, 255 }, 
	blackColor = Color{ 238, 238, 210, 255 },
	legalColor = YELLOW,
	highlightColor = RED;

Texture2D pieceSprite;
Rectangle pieces[12];

// Util reference
Utils utils = Utils();
Chess chess= Chess();

// Current pieces
std::vector<Utils::ChessPiece> myPieces;

std::vector<Vector2> legalMoves;
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
			myPieces = chess.effectuateMove(myPieces, highlight, target);

			legalMoves.clear();
			highlight = Vector2{ -1, -1 };

			if (!Chess::turn) {

				// Get bot's move
				std::tuple<Vector2, Vector2> move = Core::next(myPieces, !Chess::turn);

				myPieces = chess.effectuateMove(myPieces, std::get<0>(move), std::get<1>(move));
			}

			return;
		}

		highlight = Vector2{
			floor(mouse.x / tileSize),
			floor(mouse.y / tileSize),
		};

		legalMoves = utils.getAllLegalPieceMoves(myPieces, highlight, false, false);
		if (!chess.isSquareOccupied(myPieces, highlight)) highlight = savedHighlight;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) { highlight = Vector2{ -1, -1 }; legalMoves.clear();}
}

void chessboard() {

	for (int i = 0; i < boardSize; i++)
		for (int j = 0; j < boardSize; j++) {

			Color col = (i + j) % 2 == 0 ? blackColor: whiteColor;
			if (i == int(highlight.x) && j == int(highlight.y)) col = highlightColor;

			for (Vector2 pos : legalMoves)
				if (i == int(pos.x) && j == int(pos.y)) {

					col = legalColor;
					break;
				}

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

	Utils::loadSettings();

	setupPieces();
	placePieces(utils.startPosition);

	// Main loop
	while (!WindowShouldClose()) {

		if(Chess::end == "") checkForClicks();

		BeginDrawing();
		
		ClearBackground(BLACK);
		//Draw board
		chessboard();

		DrawText("Chessboard interface", tileSize * boardSize + 5, 10, 20, LIGHTGRAY);
		DrawText(Chess::turn ? "White to move" : "Black to move", tileSize * boardSize + 5, 32, 16, WHITE);
		DrawText(Chess::wCheck ? "WHITE KING IN CHECK" : (Chess::bCheck ? "BLACK KING IN CHECK": ""), tileSize * boardSize + 5, 50, 16, RED);

		// Show settings
		DrawText("------------ Settings ------------", tileSize * boardSize + 5, 70, 16, LIGHTGRAY);
		DrawText(std::string("Playing as: " + std::string(Utils::appSettings.colour ? "WHITE" : "BLACK")).c_str(), tileSize * boardSize + 5, 90, 16, LIGHTGRAY);

		if (Chess::end != "") {

			// Draw losing screen
			DrawRectangle(10, 10, 300, 70, WHITE);
			DrawText(Chess::end.c_str(), 15, 15, 20, BLACK);
		}

		if (IsKeyPressed(KEY_R)) {

			Chess::history.clear();
			placePieces(utils.startPosition);
			Chess::end = "";
		}

		EndDrawing();
	}

	// Flush data
	CloseWindow();

	return 0;
}