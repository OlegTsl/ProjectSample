#pragma once

namespace game::stats {

    struct StatEntry {
        int   base       = 0;
        int   flatBonus  = 0;
        float multiplier = 1.0f;

        int value() const {
            return static_cast<int>((base + flatBonus) * multiplier);
        }
    };

} // namespace game::stats