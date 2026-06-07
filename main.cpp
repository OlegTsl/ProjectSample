#include "game/grid/Grid.hpp"
#include "game/components/Identity.hpp"
#include "game/core/Context.hpp"
#include "game/core/SystemRegistry.hpp"
#include "game/systems/UnitMovementSystem.hpp"
#include "game/systems/UnitTargetSystem.hpp"
#include "game/systems/UnitSpawnSystem.hpp"
#include "game/systems/UnitAttackSystem.hpp"
#include "game/systems/UnitDeathSystem.hpp"
#include "game/systems/UnitSkillSystem.hpp"
#include "game/systems/UnitDamageSystem.hpp"

#include <iostream>
#include <string>

int getWinnerTeam(game::core::Context& context) {
    auto* teams = context.getPool<game::components::Team>();
    if (!teams)
        return -1;

    int winner = teams->data()[0].value;

    for (auto& team : teams->data()) {
        if (team.value != winner)
            return -1;
    }

    return winner;
}

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
        std::make_unique<UnitSkillSystem>(),
        std::make_unique<UnitAttackSystem>(),
        std::make_unique<UnitDamageSystem>(),
        std::make_unique<UnitMovementSystem>(grid),
        std::make_unique<UnitDeathSystem>(grid)
    );

    systemRegistry.initializeAll(context);

    std::cout << "Units after spawn: " << unitRegistry.getAll().size() << std::endl;

    while (true) {
        systemRegistry.updateAll(context, 1.0f);
        std::cout << "\n";

        int winner = getWinnerTeam(context);
        if (winner != -1) {
            std::cout << "Team " << winner << " wins!\n";
            break;
        }
    }

    return 0;
}
