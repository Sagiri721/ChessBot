#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctype.h>

#include "utils.h"
#include "chess.h"

Chess u_chess = Chess();

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::vector<Utils::ChessPiece> Utils::parseFenStringToBoardInformation(std::string fenString) {

	std::cout << "Parsing " << fenString << std::endl << std::endl;
    std::vector<std::string> rows = split(fenString, "/");

    // Output variable
    std::vector<ChessPiece> piecesOutput;
    
    int squareCounting = 0;
    int rowCounter = 0;

    for (std::string row : rows) {

        // std::cout << row << std::endl;
        // Iterate each letter
        for (int i = 0; i < row.length(); i++) {

            char curLetter = row.at(i);
            if (isalpha(curLetter)) {

                // Register piece
                int index = 0;

                if (tolower(curLetter) == tolower(*"k")) index = 0;
                if (tolower(curLetter) == tolower(*"q")) index = 1;
                if (tolower(curLetter) == tolower(*"b")) index = 2;
                if (tolower(curLetter) == tolower(*"n")) index = 3;
                if (tolower(curLetter) == tolower(*"r")) index = 4;
                if (tolower(curLetter) == tolower(*"p")) index = 5;

                if (tolower(curLetter) == int(curLetter)) index += 6;

                ChessPiece thisPiece;
                thisPiece.index = index;
                thisPiece.color = index >= 6;

                thisPiece.position = Vector2{ float(squareCounting), float(rowCounter) };
                piecesOutput.push_back(thisPiece);


                squareCounting++;
            }
            else {

                // Save offset
                int offset = std::stoi(std::string(1, curLetter));
                squareCounting += offset;
            }
        }

        squareCounting = 0;
        rowCounter++;
    }

    updateOccupationMask(piecesOutput);
	return piecesOutput;
};

int Utils::findPieceIndexFromPosition(std::vector<Utils::ChessPiece> pieces, Vector2 position) {

    //std::cout << "Finding piece at " << position.x << ", " << position.y << std::endl;

    int index = 0;
    for (Utils::ChessPiece p : pieces) {

        if (p.position.x == position.x && p.position.y == position.y) return index;
        index++;
    }

    return -1;
}

Utils::ChessPiece* Utils::findPieceFromPosition(std::vector<Utils::ChessPiece> pieces, Vector2 position) {

    for (Utils::ChessPiece p : pieces) if (p.position.x == position.x && p.position.y == position.y) return &p;
    return NULL;
}

std::vector<Utils::ChessPiece> Utils::findPiecesFromPieceIndex(std::vector<Utils::ChessPiece> pieces, int index) {

    std::vector<Utils::ChessPiece> found;

    for (Utils::ChessPiece p : pieces) if (p.index == index) found.push_back(p);
    return found;
}

std::vector<Vector2> Utils::getAllLegalPieceMoves(std::vector<Utils::ChessPiece> pieces, Vector2 origin) {

    std::vector<Vector2> found;

    Utils::ChessPiece* originPiece = findPieceFromPosition(pieces, origin);
    if (originPiece == NULL) return found;
    if (originPiece->color == Chess::turn) return found;

    int index = 0;
    bool blocked[] = {false, false, false, false};
    switch (originPiece->index)
    {
        case 0:
        case 6:

            found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y + 1 });
            found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y });
            found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y - 1});
            found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y + 1 });
            found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y });
            found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y - 1 });
            found.push_back(Vector2{ originPiece->position.x, originPiece->position.y - 1 });
            found.push_back(Vector2{ originPiece->position.x, originPiece->position.y + 1 });

            break;
        case 4:
        case 10:
            
            index = 1;
            while (index < 8) {

                if (!blocked[0]) found.push_back(Vector2{ originPiece->position.x + index, originPiece->position.y});
                if (Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y)]) blocked[0] = true;

                if (!blocked[1]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y});
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y)]) blocked[1] = true;

                if (!blocked[2]) found.push_back(Vector2{ originPiece->position.x, originPiece->position.y + index });
                if (Chess::occupationMask[int(originPiece->position.x)][int(originPiece->position.y + index)]) blocked[2] = true;

                if (!blocked[3]) found.push_back(Vector2{ originPiece->position.x, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x)][int(originPiece->position.y - index)]) blocked[3] = true;

                index++;
            }

            blocked[0] = false;
            blocked[1] = false;
            blocked[2] = false;
            blocked[3] = false;

            break;
        case 2:
        case 8:

            index = 1;
            while (index < 8) {

                if(!blocked[0]) found.push_back(Vector2{originPiece->position.x + index, originPiece->position.y + index});
                if(Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y + index)]) blocked[0] = true;

                if (!blocked[1]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y - index)]) blocked[1] = true;

                if (!blocked[2]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y + index });
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y + index)]) blocked[2] = true;

                if (!blocked[3]) found.push_back(Vector2{ originPiece->position.x + index, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y - index)]) blocked[3] = true;
                index++;
            }

            blocked[0] = false;
            blocked[1] = false;
            blocked[2] = false;
            blocked[3] = false;

            break;
        case 3:
        case 9:

            found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y + 2 });
            found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y + 2 });
            found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y - 2 });
            found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y - 2 });
            found.push_back(Vector2{ originPiece->position.x + 2, originPiece->position.y + 1 });
            found.push_back(Vector2{ originPiece->position.x + 2, originPiece->position.y - 1 });
            found.push_back(Vector2{ originPiece->position.x - 2, originPiece->position.y + 1 });
            found.push_back(Vector2{ originPiece->position.x - 2, originPiece->position.y - 1 });

            break;

        case 1:
        case 7:

            index = 1;
            while (index < 8) {

                if (!blocked[0]) found.push_back(Vector2{ originPiece->position.x + index, originPiece->position.y });
                if (Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y)]) blocked[0] = true;

                if (!blocked[1]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y });
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y)]) blocked[1] = true;

                if (!blocked[2]) found.push_back(Vector2{ originPiece->position.x, originPiece->position.y + index });
                if (Chess::occupationMask[int(originPiece->position.x)][int(originPiece->position.y + index)]) blocked[2] = true;

                if (!blocked[3]) found.push_back(Vector2{ originPiece->position.x, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x)][int(originPiece->position.y - index)]) blocked[3] = true;

                index++;
            }

            blocked[0] = false;
            blocked[1] = false;
            blocked[2] = false;
            blocked[3] = false;

            index = 1;
            while (index < 8) {

                if (!blocked[0]) found.push_back(Vector2{ originPiece->position.x + index, originPiece->position.y + index });
                if (Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y + index)]) blocked[0] = true;

                if (!blocked[1]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y - index)]) blocked[1] = true;

                if (!blocked[2]) found.push_back(Vector2{ originPiece->position.x - index, originPiece->position.y + index });
                if (Chess::occupationMask[int(originPiece->position.x - index)][int(originPiece->position.y + index)]) blocked[2] = true;

                if (!blocked[3]) found.push_back(Vector2{ originPiece->position.x + index, originPiece->position.y - index });
                if (Chess::occupationMask[int(originPiece->position.x + index)][int(originPiece->position.y - index)]) blocked[3] = true;
                index++;
            }

            break;
        case 5:
        case 11:

            found.push_back(Vector2{ originPiece->position.x, originPiece->position.y + (1 * originPiece->color ? 1: -1) });
            if(!originPiece->moved) found.push_back(Vector2{ originPiece->position.x, originPiece->position.y + (1 * originPiece->color ? 2 : -2) });

            // Add pawn capture squares
            if(Chess::occupationMask[int(originPiece->position.x + 1)][int(originPiece->position.y + (1 * originPiece->color ? 1 : -1))]) 
                found.push_back(Vector2{ originPiece->position.x + 1, originPiece->position.y + (1 * originPiece->color ? 1 : -1) });

            if (Chess::occupationMask[int(originPiece->position.x - 1)][int(originPiece->position.y + (1 * originPiece->color ? 1 : -1))])
                found.push_back(Vector2{ originPiece->position.x - 1, originPiece->position.y + (1 * originPiece->color ? 1 : -1) });

            break;
    }

    // Crop outside pieces
    std::vector<Vector2> cropped;
    for (Vector2 position : found)
    {
        if (position.x < 0 || position.x >= 8) continue;
        if (position.y < 0 || position.y >= 8) continue;

        if (Chess::occupationMask[int(position.x)][int(position.y)] && Chess::colorMask[int(position.x)][int(position.y)] != Chess::turn) continue;

        cropped.push_back(position);
    }

    return cropped;
}

void Utils::updateOccupationMask(std::vector<Utils::ChessPiece> pieces) {

    // Fill matrix with zeros
    std::fill_n(&Chess::occupationMask[0][0], sizeof(Chess::occupationMask) / sizeof(**Chess::occupationMask), 0);
    std::fill_n(&Chess::colorMask[0][0], sizeof(Chess::colorMask) / sizeof(**Chess::colorMask), -1);

    for (ChessPiece piece : pieces) {

        Chess::occupationMask[int(piece.position.x)][int(piece.position.y)] = true;
        Chess::colorMask[int(piece.position.x)][int(piece.position.y)] = piece.color;
    }

    //for (int i = 0; i < 8; i++) { for (int j = 0; j < 8; j++) { std::cout << Chess::colorMask[j][i] << " "; } std::cout << std::endl; }
}