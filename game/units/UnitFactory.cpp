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

    Unit* UnitFactory::createWarrior(tools::Vec2 pos, int team) {
        auto entity = _context.createEntity(
            components::UnitType  {components::UnitClass::Warrior},
            components::Name      {"Aragorn"},
            components::Team      {team},
            components::Health    {120, 120},
            components::Strength  {10},
            components::BaseDamage{12},
            components::Position  {pos }
        );

        return registerUnit(entity);
    }

    Unit* UnitFactory::createArcher(tools::Vec2 pos, int team) {
        auto entity = _context.createEntity(
            components::UnitType   {components::UnitClass::Archer},
            components::Name       {"Legolas"},
            components::Team       {team},
            components::Health     {80, 80},
            components::Agility    {15},
            components::BaseDamage {15},
            components::AttackRange{4},
            components::Position   {pos }
        );

        return registerUnit(entity);
    }

    Unit* UnitFactory::createMage(tools::Vec2 pos, int team) {
        auto entity = _context.createEntity(
            components::UnitType   {components::UnitClass::Mage},
            components::Name       {"Gendalf"},
            components::Team       {team},
            components::Health     {60, 60},
            components::Mana       {100, 100},
            components::Intellect  {20},
            components::BaseDamage {20},
            components::AttackRange{3},
            components::Position   {pos }
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