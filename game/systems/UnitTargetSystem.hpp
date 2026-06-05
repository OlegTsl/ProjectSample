#pragma once

#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/components/Transform.hpp"
#include "game/components/Identity.hpp"
#include "game/grid/Grid.hpp"

namespace game::systems {

    class UnitTargetSystem : public core::UpdateSystem {
    public:
        explicit UnitTargetSystem(const Grid& grid);
        void update(core::Context& ctx, float dt) override;

    private:
        const Grid& _grid;

        static std::optional<core::Entity> getTarget(
            core::Entity                               entity,
            const components::Position&                position,
            const components::Team&                    team,
            core::ComponentPool<components::Position>& positions,
            core::ComponentPool<components::Team>&     teams
        );
    };

} // namespace game::systems