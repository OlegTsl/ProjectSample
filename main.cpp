#include <iostream>
#include <string>

#include "game/components/Identity.hpp"
#include "game/components/Resources.hpp"
#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/systems/UnitSpawnSystem.hpp"
#include "game/units/Unit.hpp"

int main() {
    using namespace game;
    using namespace game::core;
    using namespace game::components;
    using namespace game::systems;

    Context        context;
    SystemRegistry systemRegistry;
    UnitRegistry   unitRegistry;
    UnitFactory    unitFactory(context, unitRegistry);

    systemRegistry.addInitialize(
        std::make_unique<UnitSpawnSystem>(unitFactory)
    );

    systemRegistry.initializeAll(context);

    std::cout << "Units after spawn: " << unitRegistry.getAll().size() << std::endl;

    for (int frame = 0; frame < 5; ++frame) {
        float dt = 1.0f / 60.0f;
        systemRegistry.updateAll(context, dt);

        auto& warriors = unitRegistry.getByType(UnitClass::Warrior);
        if (!warriors.empty()) {
            auto* health = warriors[0]->health();
            std::cout << "Frame " << frame << " - Warrior HP: "
                      << health->current << "/" << health->max << std::endl;
        }
    }
    
    return 0;
}