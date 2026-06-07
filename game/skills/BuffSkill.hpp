#pragma once

#include "game/stats/StatType.hpp"

namespace game::skills {

    struct BuffSkill {
        stats::StatType stat;
        float           multiplier;
        int             duration;
        int             cooldown;
    };

} // namespace game::skills