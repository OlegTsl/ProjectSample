#include "UnitSkillSystem.hpp"
#include "game/components/Attributes.hpp"
#include "game/components/Combat.hpp"
#include "game/components/Transform.hpp"
#include "game/components/Identity.hpp"

#include <vector>
#include <iostream>

namespace game::systems {

    void UnitSkillSystem::cleanupSkillMarkers(core::Context& context) {
        auto* markers = context.getPool<components::SkillUsed>();
        if (!markers)
            return;

        std::vector toClean(
            markers->entities().begin(),
            markers->entities().end()
        );

        for (auto entity : toClean)
            context.removeComponent<components::SkillUsed>(entity);
    }

    void UnitSkillSystem::processBuffSkill(
        core::Context&                  context,
        core::Entity                    entity,
        components::BuffSkillComponent& buffSkill)
    {
        if (buffSkill.cooldown > 0)
            return;

        if (context.hasComponent<components::StatModifier>(entity))
            return;

        context.addComponent<components::StatModifier>(entity, {
            buffSkill.skill.stat,
            buffSkill.skill.multiplier,
            buffSkill.skill.duration
        });

        buffSkill.cooldown = buffSkill.skill.cooldown;
        context.addComponent<components::SkillUsed>(entity, {});

        auto* name = context.getComponent<components::Name>(entity);
        std::cout << "[Skill] entity(" << name->value << ")"
                  << " activated buff x" << buffSkill.skill.multiplier
                  << " for " << buffSkill.skill.duration << " turns\n";
    }

    int UnitSkillSystem::calculateDamage(
        core::Context&  context,
        core::Entity    entity,
        stats::StatType stat,
        float           multiplier)
    {
        auto* stats = context.getComponent<components::BaseStats>(entity);
        auto statValue = stats ? stats->stats.value(stat) : 0;

        return static_cast<int>(statValue * multiplier);
    }

    void UnitSkillSystem::applyDamage(
        core::Context& context,
        core::Entity   entity,
        int            damage)
    {
        auto* incomingDamage = context.getComponent<components::Damage>(entity);
        if (!incomingDamage)
            incomingDamage = context.addComponent<components::Damage>(entity, {});

        incomingDamage->value += damage;
    }

    void UnitSkillSystem::processDamageSkill(
        core::Context&                    context,
        core::Entity                      entity,
        components::DamageSkillComponent& damageSkill)
    {
        if (damageSkill.cooldown > 0)
            return;

        auto* target = context.getComponent<components::Target>(entity);
        if (!target)
            return;

        auto* position       = context.getComponent<components::Position>(entity);
        auto* targetPosition = context.getComponent<components::Position>(target->entity);

        if (!position || !targetPosition)
            return;

        int distance = position->value.distance(targetPosition->value);

        auto* range    = context.getComponent<components::AttackRange>(entity);
        int   maxRange = range ? range->value : 1;

        if (distance > maxRange)
            return;

        int damage = calculateDamage(context, entity, damageSkill.skill.stat, damageSkill.skill.multiplier);
        applyDamage(context, target->entity, damage);

        damageSkill.cooldown = damageSkill.skill.cooldown;
        context.addComponent<components::SkillUsed>(entity, {});

        auto* name       = context.getComponent<components::Name>(entity);
        auto* targetName = context.getComponent<components::Name>(target->entity);

        std::cout << "[Skill] entity(" << name->value << ")"
                  << " skill hit entity(" << targetName->value << ")"
                  << " for " << damage << " dmg\n";
    }

    void UnitSkillSystem::updateCooldowns(core::Context& context) {
        auto* buffPool = context.getPool<components::BuffSkillComponent>();
        if (buffPool) {
            for (auto entity : buffPool->entities()) {
                auto* buffSkill = buffPool->get(entity);
                if (buffSkill && buffSkill->cooldown > 0)
                    buffSkill->cooldown--;
            }
        }

        auto* damagePool = context.getPool<components::DamageSkillComponent>();
        if (damagePool) {
            for (auto entity : damagePool->entities()) {
                auto* damageSkill = damagePool->get(entity);
                if (damageSkill && damageSkill->cooldown > 0)
                    damageSkill->cooldown--;
            }
        }
    }

    void UnitSkillSystem::updateBuffs(core::Context& context) {
        auto* modifiers = context.getPool<components::StatModifier>();
        if (!modifiers)
            return;

        std::vector<core::Entity> expired;

        for (auto entity : modifiers->entities()) {
            auto* modifier = modifiers->get(entity);
            if (!modifier)
                continue;

            modifier->remain--;

            if (modifier->remain <= 0)
                expired.push_back(entity);
        }

        for (auto entity : expired) {
            context.removeComponent<components::StatModifier>(entity);

            auto* name = context.getComponent<components::Name>(entity);
            std::cout << "[Skill] entity(" << name->value << ") buff expired\n";
        }
    }

    void UnitSkillSystem::update(core::Context& context, float dt) {
        cleanupSkillMarkers(context);

        auto* buffPool   = context.getPool<components::BuffSkillComponent>();
        auto* damagePool = context.getPool<components::DamageSkillComponent>();

        if (buffPool) {
            for (const auto entity : buffPool->entities()) {
                if (auto* buffSkill = buffPool->get(entity))
                    processBuffSkill(context, entity, *buffSkill);
            }
        }

        if (damagePool) {
            for (const auto entity : damagePool->entities()) {
                if (auto* damageSkill = damagePool->get(entity))
                    processDamageSkill(context, entity, *damageSkill);
            }
        }

        updateCooldowns(context);
        updateBuffs(context);
    }

} // namespace game::systems