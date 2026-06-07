#include "UnitAttackSystem.hpp"

#include "game/components/Skills.hpp"
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

    int UnitAttackSystem::calculateDamage(
        core::Context&  context,
        core::Entity    entity,
        stats::StatType stat)
    {
        auto* stats      = context.getComponent<components::BaseStats>(entity);
        auto* modifier   = context.getComponent<components::StatModifier>(entity);
        auto* damageMult = context.getComponent<components::DamageMultiplier>(entity);

        auto statValue = stats ? stats->stats.value(stat) : 0;
        auto statMult  = (modifier && modifier->stat == stat) ? modifier->multiplier : 1.0f;
        auto multValue = damageMult ? damageMult->value : 1.0f;

        return static_cast<int>(statValue * statMult * multValue);
    }

    void UnitAttackSystem::applyDamage(
        core::Context& context,
        core::Entity   entity,
        int            damage)
    {
        auto* incomingDamage = context.getComponent<components::Damage>(entity);
        if (!incomingDamage)
            incomingDamage = context.addComponent<components::Damage>(entity, {});

        incomingDamage->value += damage;
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
        if (context.hasComponent<components::SkillUsed>(entity))
            return;

        auto* targetPosition = context.getComponent<components::Position>(target.entity);
        if (!targetPosition)
            return;

        int distance = position.value.distance(targetPosition->value);

        auto* melee  = context.getComponent<components::MeleeAttack>(entity);
        auto* ranged = context.getComponent<components::RangedAttack>(entity);

        int damage = 0;

        if (distance == 1 && melee) {
            damage = calculateDamage(context, entity, melee->stat);
        } else if (distance > 1 && ranged) {
            auto* range = context.getComponent<components::AttackRange>(entity);
            if (!range || distance > range->value)
                return;

            damage = calculateDamage(context, entity, ranged->stat);
        } else {
            return;
        }

        damage = applyCrit(context, entity, damage);
        applyDamage(context, target.entity, damage);

        auto* entityName = context.getComponent<components::Name>(entity);
        auto* targetName = context.getComponent<components::Name>(target.entity);

        std::cout << "[Attack] entity(" << entityName->value << ")"
                  << " hits entity(" << targetName->value << ")"
                  << " for " << damage << " dmg\n";
    }

} // namespace game::systems