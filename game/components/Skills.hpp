#pragma once

#include "game/skills/BuffSkill.hpp"
#include "game/skills/DamageSkill.hpp"

namespace game::components
{
    struct BuffSkillComponent {
        skills::BuffSkill skill;
        int               cooldown = 0;
    };

    struct DamageSkillComponent {
        skills::DamageSkill skill;
        int                 cooldown = 0;
    };

    struct StatModifier {
        stats::StatType stat;
        float           multiplier;
        int             remain;
    };

    struct SkillUsed {};
} // namespace game::components
