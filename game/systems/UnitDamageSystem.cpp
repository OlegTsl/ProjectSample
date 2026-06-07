#include "UnitDamageSystem.hpp"
#include "game/components/Resources.hpp"
#include "game/components/Identity.hpp"

#include <iostream>

namespace game::systems {

    void UnitDamageSystem::applyDamage(
    core::Context&      context,
    core::Entity        entity,
    components::Damage& damage)
    {
        auto* health = context.getComponent<components::Health>(entity);
        if (!health)
            return;

        health->current -= damage.value;

        auto* name = context.getComponent<components::Name>(entity);
        std::cout << "[Damage] entity(" << name->value << ")"
                  << " took " << damage.value << " dmg"
                  << " | HP: " << health->current
                  << "/" << health->max << "\n";

        damage.value = 0;
        context.removeComponent<components::Damage>(entity);
    }

    void UnitDamageSystem::update(core::Context& context, float dt) {
        auto* damagePool = context.getPool<components::Damage>();
        if (!damagePool)
            return;

        std::vector toProcess(
            damagePool->entities().begin(),
            damagePool->entities().end()
        );

        for (auto entity : toProcess)
            applyDamage(context, entity, *damagePool->get(entity));
    }

} // namespace game::systems