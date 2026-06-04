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
        std::optional<Vec2> getMovePosition(const Vec2& from, const Vec2& to, int range) const;

        Grid& _grid;
    };

} // namespace game::systems