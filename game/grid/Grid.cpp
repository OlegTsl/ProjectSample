#include "Grid.hpp"
#include <cassert>

namespace game {

    bool Grid::inBounds(Vec2 pos) {
        return pos.x >= 0 && pos.x < WIDTH
            && pos.y >= 0 && pos.y < HEIGHT;
    }

    bool Grid::isFree(Vec2 pos) const {
        return inBounds(pos) && getCell(pos).isFree();
    }

    const Cell& Grid::getCell(Vec2 pos) const {
        assert(inBounds(pos));
        return _cells[index(pos)];
    }

    bool Grid::place(core::Entity entity, Vec2 pos) {
        if (!isFree(pos))
            return false;

        cellAt(pos).entity = entity;
        return true;
    }

    void Grid::remove(core::Entity entity, Vec2 pos) {
        assert(inBounds(pos));
        assert(getCell(pos).entity == entity);
        cellAt(pos).entity = std::nullopt;
    }

    bool Grid::move(core::Entity entity, Vec2 from, Vec2 to) {
        if (!isFree(to))
            return false;

        remove(entity, from);
        place(entity, to);
        return true;
    }

    std::vector<Vec2> Grid::getNeighbours(Vec2 pos) {
        std::vector<Vec2> result;
        result.reserve(4);

        for (auto& dir : DIRECTIONS) {
            Vec2 n{pos.x + dir.x, pos.y + dir.y};
            if (inBounds(n)) result.push_back(n);
        }

        return result;
    }

} // namespace game