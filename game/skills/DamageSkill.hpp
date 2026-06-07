#pragma once

namespace game::skills {

    struct DamageSkill {
        stats::StatType stat;
        float           multiplier;
        int             cooldown;
    };
    
} // namespace game::skills