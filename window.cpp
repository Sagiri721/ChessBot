#include <iostream>
#include <raylib.h>
#include <string>
#include <vector>
#include <tuple>
#include <iomanip>

#include "core/utils.h"
#include "core/chess.h"
#include "core/core.h"

const int boardSize = 8;
const int tileSize = 70;

float whiteEval = 1, blackEval = 1;

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

void clockTicker() {

	if (GetFPS() > 0) {

		if (Chess::end != "") return;

		double timeDifference = float(1) / GetFPS();

		if(Chess::turn) Chess::whiteTime -= timeDifference;
		else Chess::blackTime -= timeDifference;

		if (Chess::whiteTime <= 0) Chess::end = "Black won on time";
		else if (Chess::blackTime <= 0) Chess::end = "White won on time";
	}
}

std::vector<Vector2> legalMoves;
Vector2 highlight = Vector2{-1, -1};
void checkForClicks() {

	if ((Utils::appSettings.colour ? !Chess::turn : Chess::turn) && Utils::appSettings.colour != 3) {

		std::cout << Utils::appSettings.colour;

		// Get bot's move
		std::tuple<Vector2, Vector2> move = Core::next(myPieces, !Chess::turn);
		myPieces = chess.effectuateMove(myPieces, std::get<0>(move), std::get<1>(move));

		whiteEval = Core::eval(myPieces, false); 
		blackEval = Core::eval(myPieces, true);
	}

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

			whiteEval = Core::eval(myPieces, false);
			blackEval = Core::eval(myPieces, true);

			legalMoves.clear();
			highlight = Vector2{ -1, -1 };

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

void changePromotionPiece() {

	if (IsKeyPressed(KEY_ONE)) Chess::promoteTo = 1;
	if (IsKeyPressed(KEY_TWO)) Chess::promoteTo = 2;
	if (IsKeyPressed(KEY_THREE)) Chess::promoteTo = 3;
	if (IsKeyPressed(KEY_FOUR)) Chess::promoteTo = 4;
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

	whiteEval = Core::eval(myPieces, false);
	blackEval = Core::eval(myPieces, true);

	// Main loop
	while (!WindowShouldClose()) {

		if (Chess::end == "") { 
			
			checkForClicks(); 
			changePromotionPiece();
		}

		BeginDrawing();
		
		ClearBackground(BLACK);
		//Draw board
		chessboard();

		DrawText("Chessboard interface", tileSize * boardSize + 5, 10, 20, LIGHTGRAY);
		DrawText(Chess::turn ? "White to move" : "Black to move", tileSize * boardSize + 5, 32, 16, WHITE);
		DrawText(Chess::wCheck ? "WHITE KING IN CHECK" : (Chess::bCheck ? "BLACK KING IN CHECK": ""), tileSize * boardSize + 5, 50, 16, RED);

		DrawText("Promoting to: ", tileSize * boardSize + 10, 520, 20, RED);
		DrawTexturePro(pieceSprite, pieces[Chess::promoteTo], Rectangle{ tileSize * boardSize + 150, 503, 50, 50}, Vector2{ 0, 0 }, 0, WHITE);

		DrawText(TextFormat("Engine evaluation:\nW: %f | B: %f", whiteEval, blackEval), tileSize * boardSize + 5, 200, 18, DARKGREEN);

		// Show settings
		DrawText("---------- Settings ----------", tileSize * boardSize + 5, 70, 18, LIGHTGRAY);
		DrawText(std::string("Playing as: " + std::string(Utils::appSettings.colour == 3 ? "BOTH" : Utils::appSettings.colour ? "WHITE" : "BLACK") + (Utils::appSettings.random ? " (random)" : "")).c_str(), tileSize * boardSize + 5, 90, 18, LIGHTGRAY);

		if (Chess::end != "") {

			// Draw losing screen
			DrawRectangle(10, 10, 300, 70, WHITE);
			DrawText(std::string(Chess::end + "\nPress R to restart").c_str(), 15, 15, 20, BLACK);
		}

		// Show time clocks
		int minutes = ((int)(Chess::whiteTime / 60)) % 60;
		int seconds = (int)(int(Chess::whiteTime) % 60);

		DrawText(TextFormat("%d:%s%d", minutes, seconds < 10 ? "0":"", seconds), tileSize * boardSize + 10, 470, 30, LIGHTGRAY);

		minutes = ((int)(Chess::blackTime / 60)) % 60;;
		seconds = (int)(int(Chess::blackTime) % 60);

		DrawText(TextFormat("%d:%s%d", minutes, seconds < 10 ? "0" : "", seconds), tileSize * boardSize + 10, 430, 30, LIGHTGRAY);

		if (IsKeyPressed(KEY_R)) {

			Chess::history.clear(); 
			Chess::turn = true;

			Chess::wCheck = false;
			Chess::bCheck = false;

			Chess::whiteTime = Utils::appSettings.rawTime;
			Chess::blackTime = Utils::appSettings.rawTime;

			if (Utils::appSettings.random) Utils::appSettings.colour = GetRandomValue(0,1);

			placePieces(utils.startPosition);
			Chess::end = "";
		}

		clockTicker();

		EndDrawing();
	}

	// Flush data
	CloseWindow();

	return 0;
}