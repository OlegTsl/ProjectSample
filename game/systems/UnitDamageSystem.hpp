#pragma once

#include "game/components/Combat.hpp"
#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"

namespace game::systems {

    class UnitDamageSystem : public core::UpdateSystem {
    public:
        void update(core::Context& context, float dt) override;

    private:
        static void applyDamage(
            core::Context&      context,
            core::Entity        entity,
            components::Damage& damage
        );
    };

} // namespace game::systems