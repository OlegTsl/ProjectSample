#pragma once

namespace game::stats {

    enum class StatType : uint8_t {
        Strength = 0,
        Agility,
        Intellect,
        Accuracy,
        CritChance,
        CritDamage,
        Count
    };

} // namespace game::stats