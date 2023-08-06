#pragma once

#include <vector>
#include <raylib.h>

#include "utils.h"

class Core {

public:
	static std::tuple<Vector2, Vector2> next(std::vector<Utils::ChessPiece> pieces, bool);
};