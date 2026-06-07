#pragma once

#include "core/Context.hpp"
#include "core/SystemRegistry.hpp"
#include "game/grid/Grid.hpp"

namespace game::systems {

    class UnitMovementSystem : public core::UpdateSystem {
    public:
        explicit UnitMovementSystem(Grid& grid);
        void update(core::Context& context, float dt) override;

    private:
        static std::optional<Vec2> getMovePosition(
            const Grid& grid,
            const Vec2& from,
            const Vec2& to,
            int         range);

        Grid& _grid;
    };

} // namespace game::systems