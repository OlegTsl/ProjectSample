#pragma once

#include <array>
#include <optional>
#include <vector>

#include "game/core/Entity.hpp"
#include "game/tools/Vec2.hpp"

namespace game {
    using namespace tools;

    struct Cell {
        std::optional<core::Entity> entity;

        bool isFree() const { return !entity.has_value(); }
    };

    class Grid {
    public:
        static constexpr int WIDTH  = 10;
        static constexpr int HEIGHT = 10;
        static constexpr int SIZE   = WIDTH * HEIGHT;

        static constexpr std::array<Vec2, 4> DIRECTIONS = {{
            { 0, -1},
            { 0,  1},
            {-1,  0},
            { 1,  0},
        }};

        Grid() = default;

        bool place (core::Entity entity, Vec2 pos);
        void remove(core::Entity entity, Vec2 pos);
        bool move  (core::Entity entity, Vec2 from, Vec2 to);

        const Cell& getCell (Vec2 pos) const;

        bool        isFree  (Vec2 pos) const;
        static bool inBounds(Vec2 pos);

        static std::vector<Vec2> getNeighbours(Vec2 pos);

    private:
        std::array<Cell, SIZE> _cells{};

        int   index (Vec2 pos) const { return pos.y * WIDTH + pos.x; }
        Cell& cellAt(Vec2 pos)       { return _cells[index(pos)]; }
    };

} // namespace game