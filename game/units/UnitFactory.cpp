#include "UnitFactory.hpp"
#include "Unit.hpp"
#include "game/components/Attributes.hpp"
#include "game/components/Resources.hpp"
#include "game/components/Transform.hpp"
#include "game/components/Combat.hpp"
#include "game/stats/Stats.hpp"

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
        stats::Stats stats;
        stats.setBase(stats::StatType::Strength,    15);
        stats.setBase(stats::StatType::Agility,      5);
        stats.setBase(stats::StatType::Intellect,    2);
        stats.setBase(stats::StatType::Accuracy,    10);
        stats.setBase(stats::StatType::CritChance,   5);
        stats.setBase(stats::StatType::CritDamage, 150);

        auto entity = _context.createEntity(
            components::Name       {"Aragorn"},
            components::Team       {team},
            components::Health     {120, 120},
            components::BaseDamage {12},
            components::MeleeAttack{stats::StatType::Strength},
            components::BaseStats  {stats},
            components::Position   {pos}
        );

        return registerUnit(entity);
    }

    Unit* UnitFactory::createArcher(tools::Vec2 pos, int team) {
        stats::Stats stats;
        stats.setBase(stats::StatType::Strength,    8);
        stats.setBase(stats::StatType::Agility,    20);
        stats.setBase(stats::StatType::Intellect,   4);
        stats.setBase(stats::StatType::Accuracy,   20);
        stats.setBase(stats::StatType::CritChance, 15);
        stats.setBase(stats::StatType::CritDamage,175);

        auto entity = _context.createEntity(
            components::Name        {"Legolas"},
            components::Team        {team},
            components::Health      {80, 80},
            components::BaseDamage  {15},
            components::AttackRange {4},
            components::MeleeAttack {stats::StatType::Strength},
            components::RangedAttack{stats::StatType::Agility},
            components::BaseStats   {stats},
            components::Position    {pos}
        );

        return registerUnit(entity);
    }

    Unit* UnitFactory::createMage(tools::Vec2 pos, int team) {
        stats::Stats stats;
        stats.setBase(stats::StatType::Strength,    3);
        stats.setBase(stats::StatType::Agility,     6);
        stats.setBase(stats::StatType::Intellect,  25);
        stats.setBase(stats::StatType::Accuracy,   15);
        stats.setBase(stats::StatType::CritChance, 10);
        stats.setBase(stats::StatType::CritDamage,200);

        auto entity = _context.createEntity(
            components::Name        {"Gendalf"},
            components::Team        {team},
            components::Health      {60, 60},
            components::Mana        {100, 100},
            components::BaseDamage  {20},
            components::AttackRange {3},
            components::RangedAttack{stats::StatType::Intellect},
            components::BaseStats   {stats},
            components::Position    {pos}
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