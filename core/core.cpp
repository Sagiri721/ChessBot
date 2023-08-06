#include <iostream>
#include <vector>
#include <raylib.h>
#include <tuple>
#include <random>

#include "core.h"
#include "utils.h"
#include "chess.h"

Utils co_utils = Utils();

int start_core()
{
    std::cout << "######## Chess bot core library start! ########" << std::endl;
    return 0;
}

template <typename T>
T randomFrom(const T min, const T max)
{
    static std::random_device rdev;
    static std::default_random_engine re(rdev());
    typedef typename std::conditional<
        std::is_floating_point<T>::value,
        std::uniform_real_distribution<T>,
        std::uniform_int_distribution<T>>::type dist_type;
    dist_type uni(min, max);
    return static_cast<T>(uni(re));
}

std::tuple<Vector2, Vector2> Core::next(std::vector<Utils::ChessPiece> pieces, bool color) {

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
        return moves.at(randomFrom<int>(0, moves.size()-1));
    }
    else
    {

        Chess::end = "Black king was checkmated\nPress r to play again";
        return std::make_tuple(Vector2{0,0}, Vector2{0,0});
    }
}