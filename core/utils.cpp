#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>
#include <ctype.h>

#include "utils.h"

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