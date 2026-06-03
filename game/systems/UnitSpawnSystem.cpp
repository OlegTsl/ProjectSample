#include "UnitSpawnSystem.hpp"

namespace game::systems {
    UnitSpawnSystem::UnitSpawnSystem(UnitFactory& factory)
        : _factory(factory) {
    }

    void UnitSpawnSystem::initialize(core::Context& ctx) {
        _factory.createWarrior(0, 0, 1);
        _factory.createArcher(5, 0, 1);
        _factory.createMage(0, 5, 2);
    }
}