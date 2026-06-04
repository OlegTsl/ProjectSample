#include "UnitSpawnSystem.hpp"
#include "game/units/Unit.hpp"

namespace game::systems {
    UnitSpawnSystem::UnitSpawnSystem(UnitFactory& factory, Grid& grid)
        : _factory(factory),
        _grid(grid)
    {
    }

    void UnitSpawnSystem::initialize(core::Context& context) {
        auto warriorPos = Vec2{0, 0};
        auto archerPos  = Vec2{0, 9};
        auto magePos    = Vec2{9, 9};

        auto* warrior = _factory.createWarrior(warriorPos, 1);
        auto* archer  = _factory.createArcher (archerPos,  2);
        auto* mage    = _factory.createMage   (magePos,    3);

        _grid.place(warrior->getEntity(), warriorPos);
        _grid.place(archer->getEntity(),  archerPos);
        _grid.place(mage->getEntity(),    magePos);
    }
}
