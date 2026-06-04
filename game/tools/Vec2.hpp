#pragma once

#include <cstdlib>

namespace game::tools {

    struct Vec2 {
        int x, y;

        bool operator==(const Vec2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Vec2& other) const { return !(*this == other);            }

        int distance(const Vec2& other) const {
            return std::abs(x - other.x) + std::abs(y - other.y);
        }
    };

} // game::tools