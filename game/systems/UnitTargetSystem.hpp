#pragma once

#include "core/Context.hpp"
#include "core/SystemRegistry.hpp"
#include "game/grid/Grid.hpp"

namespace game::systems {

    class UnitTargetSystem : public core::UpdateSystem {
    public:
        explicit UnitTargetSystem(const Grid& grid);
        void update(core::Context& ctx, float dt) override;

    private:
        const Grid& _grid;
    };

} // namespace game::systems