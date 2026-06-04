#include "game/grid/Grid.hpp"
#include "game/components/Identity.hpp"
#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/systems/UnitMovementSystem.hpp"
#include "game/systems/UnitTargetSystem.hpp"
#include "game/systems/UnitSpawnSystem.hpp"

#include <iostream>
#include <string>

int main() {
    using namespace game;
    using namespace game::core;
    using namespace game::components;
    using namespace game::systems;

    Context        context;
    SystemRegistry systemRegistry;
    UnitRegistry   unitRegistry;
    UnitFactory    unitFactory(context, unitRegistry);

    Grid grid;

    systemRegistry.addInitialize(
        std::make_unique<UnitSpawnSystem>(unitFactory, grid)
    );

    systemRegistry.addUpdate(
        std::make_unique<UnitTargetSystem>(grid),
        std::make_unique<UnitMovementSystem>(grid)
    );

    systemRegistry.initializeAll(context);

    std::cout << "Units after spawn: " << unitRegistry.getAll().size() << std::endl;

    for (int frame = 0; frame < 15; ++frame) {
        float dt = 1.0f / 60.0f;
        systemRegistry.updateAll(context, dt);
    }
    
    return 0;
}