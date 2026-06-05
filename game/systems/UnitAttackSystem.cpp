#include "UnitAttackSystem.hpp"

#include "game/components/Resources.hpp"
#include "game/components/Identity.hpp"

#include <iostream>

namespace game::systems {

    void UnitAttackSystem::update(core::Context& context, float dt) {
        auto* targets   = context.getPool<components::Target>();
        auto* positions = context.getPool<components::Position>();

        if (!targets || !positions)
            return;

        for (auto entity : targets->entities()) {
            auto* position = positions->get(entity);
            auto* target   = targets->get(entity);

            if (!position || !target)
                continue;

            processEntity(context, entity, *position, *target);
        }
    }

    int UnitAttackSystem::resolveAttack(
        core::Context&  context,
        core::Entity    entity,
        stats::StatType stat)
    {
        auto* stats  = context.getComponent<components::BaseStats>(entity);
        auto* damage = context.getComponent<components::BaseDamage>(entity);

        int statValue   = stats  ? stats->stats.value(stat) : 0;
        int damageValue = damage ? damage->value            : 0;

        return statValue + damageValue;
    }

    int UnitAttackSystem::applyCrit(
        core::Context& context,
        core::Entity   entity,
        int            damage)
    {
        auto* stats = context.getComponent<components::BaseStats>(entity);
        if (!stats)
            return damage;

        int critChance = stats->stats.value(stats::StatType::CritChance);
        int roll       = std::rand() % 100;

        if (roll >= critChance)
            return damage;

        float critMultiplier = stats->stats.value(stats::StatType::CritDamage) / 100.0f;
        int   critDamage     = static_cast<int>(damage * critMultiplier);

        std::cout << "[Crit!] x" << critMultiplier
                  << " (" << damage << " -> " << critDamage << ")\n";

        return critDamage;
    }

    void UnitAttackSystem::processEntity(
        core::Context&              context,
        core::Entity                entity,
        const components::Position& position,
        const components::Target&   target)
    {
        auto* targetPosition = context.getComponent<components::Position>(target.entity);
        auto* targetHealth   = context.getComponent<components::Health>(target.entity);

        if (!targetPosition || !targetHealth)
            return;

        int distance = position.value.distance(targetPosition->value);

        auto* melee  = context.getComponent<components::MeleeAttack>(entity);
        auto* ranged = context.getComponent<components::RangedAttack>(entity);

        int damage = 0;

        if (distance == 1 && melee) {
            damage = resolveAttack(context, entity, melee->stat);
        } else if (distance > 1 && ranged) {
            auto* range = context.getComponent<components::AttackRange>(entity);
            if (!range || distance > range->value)
                return;

            damage = resolveAttack(context, entity, ranged->stat);
        } else {
            return;
        }

        damage = applyCrit(context, entity, damage);
        targetHealth->current -= damage;

        auto* entityName = context.getComponent<components::Name>(entity);
        auto* targetName = context.getComponent<components::Name>(target.entity);

        std::cout << "[Attack] entity(" << entityName->value << ")"
                  << " hits entity(" << targetName->value << ")"
                  << " for " << damage << " dmg"
                  << " | HP: " << targetHealth->current
                  << "/" << targetHealth->max << "\n";
    }

} // namespace game::systems