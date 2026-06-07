#pragma once

#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/components/Skills.hpp"

namespace game::systems {

    class UnitSkillSystem : public core::UpdateSystem {
    public:
        void update(core::Context& context, float dt) override;

    private:
        static void cleanupSkillMarkers(core::Context& context);

        static void processBuffSkill(
            core::Context&                  context,
            core::Entity                    entity,
            components::BuffSkillComponent& buffSkill
        );

        static void processDamageSkill(
            core::Context&                    context,
            core::Entity                      entity,
            components::DamageSkillComponent& damageSkill
        );

        static int calculateDamage(
            core::Context&  context,
            core::Entity    entity,
            stats::StatType stat,
            float           multiplier
        );

        static void applyDamage(
            core::Context& context,
            core::Entity   entity,
            int            damage
        );

        static void updateCooldowns(core::Context& context);
        static void updateBuffs    (core::Context& context);
    };

} // namespace game::systems