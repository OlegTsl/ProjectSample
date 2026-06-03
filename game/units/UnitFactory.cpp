#include "UnitFactory.hpp"
#include "Unit.hpp"
#include "components/Attributes.hpp"
#include "components/Resources.hpp"
#include "components/Transform.hpp"
#include "components/Combat.hpp"

namespace game {
    UnitFactory::UnitFactory(core::Context& context, UnitRegistry& registry)
        : _context(context)
        , _registry(registry)
    {
        _context.onDestroy([this](core::Entity entity) {
            _registry.unregisterUnit(entity);
        });
    }

    Unit* UnitFactory::createWarrior(int x, int y, int team) {
        auto entity = _context.createEntity(
            components::UnitClass::Warrior,
            components::Team     {team},
            components::Health   {120, 120},
            components::Strength {15},
            components::Agility  {5},
            components::Intellect{2},
            components::Damage   {12},
            components::Position {x, y});

        return registerUnit(entity);
    }

    Unit* UnitFactory::createArcher(int x, int y, int team) {
        auto entity = _context.createEntity(
            components::UnitClass::Archer,
            components::Team{team},
            components::Health{80, 80},
            components::Strength{8},
            components::Agility{20},
            components::Intellect{4},
            components::Damage{15},
            components::Position{x, y});

        return registerUnit(entity);
    }

    Unit* UnitFactory::createMage(int x, int y, int team) {
        auto entity = _context.createEntity(
            components::UnitClass::Mage,
            components::Team{team},
            components::Health{60, 60},
            components::Mana{100, 100},
            components::Strength{3},
            components::Agility{6},
            components::Intellect{25},
            components::Damage{30},
            components::Position{x, y}
        );

        return registerUnit(entity);
    }

    Unit* UnitFactory::registerUnit(core::Entity& entity) {
        auto unit = std::make_unique<Unit>(entity, _context);
        Unit* ptr = unit.get();
        _registry.registerUnit(std::move(unit));

        return ptr;
    }
} // namespace game