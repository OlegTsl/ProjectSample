#pragma once

#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/components/Combat.hpp"
#include "game/components/Attributes.hpp"
#include "game/components/Transform.hpp"

namespace game::systems {

    class UnitAttackSystem : public core::UpdateSystem {
    public:
        void update(core::Context& context, float dt) override;

    private:
        static void processEntity(
            core::Context&              context,
            core::Entity                entity,
            const components::Position& position,
            const components::Target&   target
        );

        static int calculateDamage(
            core::Context&  context,
            core::Entity    entity,
            stats::StatType stat
        );

        static void applyDamage(
            core::Context& context,
            core::Entity   entity,
            int            damage
        );

        static int applyCrit(
            core::Context& context,
            core::Entity   entity,
            int            damage
        );
    };

} // namespace game::systems