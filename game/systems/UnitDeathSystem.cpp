#include "UnitDeathSystem.hpp"

#include "game/components/Transform.hpp"
#include "game/components/Identity.hpp"

#include <vector>
#include <iostream>

namespace game::systems {

    UnitDeathSystem::UnitDeathSystem(Grid& grid)
        : _grid(grid)
    {}

    void UnitDeathSystem::update(core::Context& context, float dt) {
        auto* healthPool = context.getPool<components::Health>();
        if (!healthPool)
            return;
        
        std::vector<core::Entity> dead;

        for (auto entity : healthPool->entities()) {
            auto* health = healthPool->get(entity);
            if (health && isDead(*health))
                dead.push_back(entity);
        }

        for (auto entity : dead)
            processEntity(context, entity, _grid);
    }

    bool UnitDeathSystem::isDead(const components::Health& health) {
        return health.current <= 0;
    }

    void UnitDeathSystem::processEntity(
        core::Context& context,
        core::Entity   entity,
        Grid&          grid)
    {
        if (auto* position = context.getComponent<components::Position>(entity))
            grid.remove(entity, position->value);

        if (auto* name = context.getComponent<components::Name>(entity))
            std::cout << "[Death] entity(" << name->value << ") removed\n";

        context.destroyEntity(entity);
    }

} // namespace game::systems