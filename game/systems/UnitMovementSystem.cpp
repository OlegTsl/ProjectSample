#include "UnitMovementSystem.hpp"

#include "game/components/Identity.hpp"
#include "game/components/Transform.hpp"
#include "game/components/Combat.hpp"

#include <iostream>

namespace game::systems {

    UnitMovementSystem::UnitMovementSystem(Grid& grid)
        : _grid(grid)
    {}

    void UnitMovementSystem::update(core::Context& context, float dt) {
        auto* positions = context.getPool<components::Position>();
        auto* targets   = context.getPool<components::Target>();

        if (!positions || !targets)
            return;

        for (auto entity : targets->entities()) {
            auto* position = positions->get(entity);
            auto* target   = targets->get(entity);

            if (!position || !target)
                continue;

            if (auto* targetPos = context.getComponent<components::Position>(target->entity)) {
                Vec2 from = position->value;
                Vec2 to   = targetPos->value;

                auto* range  = context.getComponent<components::AttackRange>(entity);
                auto movePos = getMovePosition(_grid, from, to, range ? range->value : 1);

                if (!movePos.has_value())
                    continue;

                auto dst = movePos.value();
                if (_grid.move(entity, from, dst)) {
                    position->value = dst;

                    auto* name = context.getComponent<components::Name>(entity);
                    std::cout << "[Move] entity(" << name->value << ") "
                          << "(" << from.x << "," << from.y << ")"
                          << " -> "
                          << "(" << dst.x << "," << dst.y << ")"
                          << " target at (" << to.x << "," << to.y << ")\n";
                }
            }
        }
    }

    std::optional<Vec2> UnitMovementSystem::getMovePosition(
        const Grid& grid,
        const Vec2& from,
        const Vec2& to,
        int         range)
    {
        if (from.distance(to) <= range)
            return std::nullopt;

        Vec2 best     = from;
        int  bestDist = std::numeric_limits<int>::max();

        for (const auto& dir : Grid::DIRECTIONS) {
            Vec2 next{from.x + dir.x, from.y + dir.y};
            if (!grid.isFree(next))
                continue;

            int dist = next.distance(to);
            if (dist < bestDist) {
                bestDist = dist;
                best     = next;
            }
        }

        return best == from ? std::nullopt : std::optional{best};
    }

} // namespace game::systems