#pragma once

#include "game/components/Resources.hpp"
#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/grid/Grid.hpp"

namespace game::systems {

    class UnitDeathSystem : public core::UpdateSystem {
    public:
        explicit UnitDeathSystem(Grid& grid);
        void update(core::Context& context, float dt) override;

    private:
        Grid& _grid;

        static bool isDead(const components::Health& health);

        static void processEntity(
            core::Context& context,
            core::Entity   entity,
            Grid&          grid
        );
    };

} // namespace game::systems