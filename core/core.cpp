#include <iostream>
#include <vector>
#include <raylib.h>
#include <tuple>

#include "core.h"
#include "utils.h"
#include "chess.h"

Utils co_utils = Utils();

int start_core()
{
    std::cout << "######## Chess bot core library start! ########" << std::endl;
    return 0;
}

std::tuple<Vector2, Vector2> Core::next(std::vector<Utils::ChessPiece> pieces, bool color) {

    std::cout << "[Engine] finding next move...\n";

    //Find all legal moves
    std::vector<std::tuple<Vector2, Vector2>> moves;
    for (Utils::ChessPiece piece : pieces) {

        if (piece.color == color) {

            std::vector<Vector2> legalMoves = co_utils.getAllLegalPieceMoves(pieces, piece.position, false, true);
            for (Vector2 move : legalMoves) {

                if (Chess::colorMask[int(move.x)][int(move.y)] == int(color)) continue;

                std::tuple<Vector2, Vector2> play = std::make_tuple(piece.position, move);
                moves.push_back(play);
            }
        }
    }

    if(moves.size() != 0)
    {
        return moves.at(GetRandomValue(0, moves.size()-1));
    }
    else
    {
        if(Utils::appSettings.colour){
            if(Chess::bCheck) Chess::end = "Black was checkmated";
            else Chess::end = "Black was stalemated";
        } else{
            if (Chess::wCheck) Chess::end = "White was checkmated";
            else Chess::end = "White was stalemated";
        }

        return std::make_tuple(Vector2{0,0}, Vector2{0,0});
    }
}

float Core::eval(std::vector<Utils::ChessPiece> pieces, bool color) {

    // Evaluate this position
    // Count material

    int myMaterial = 0;
    for (Utils::ChessPiece piece : pieces) {

        if (piece.color == color) {

            myMaterial += pieceValueMap.find(piece.index)->second;
        }
    }
    float materialBalance = float(myMaterial) / 59;

    return materialBalance;
}