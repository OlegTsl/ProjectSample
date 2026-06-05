#pragma once

#include "StatEntry.hpp"
#include "StatType.hpp"

#include <array>

namespace game::stats {

    struct Stats {
    private:
        static constexpr size_t COUNT = static_cast<size_t>(StatType::Count);
        std::array<StatEntry, COUNT> _entries{};

    public:
        int value(StatType type) const {
            return _entries[static_cast<size_t>(type)].value();
        }

        void setBase(StatType type, int value) {
            _entries[static_cast<size_t>(type)].base = value;
        }

        void addFlatBonus(StatType type, int bonus) {
            _entries[static_cast<size_t>(type)].flatBonus += bonus;
        }

        void setMultiplier(StatType type, float multiplier) {
            _entries[static_cast<size_t>(type)].multiplier = multiplier;
        }

        void multiplyBy(StatType type, float factor) {
            _entries[static_cast<size_t>(type)].multiplier *= factor;
        }

        const StatEntry& entry(StatType type) const {
            return _entries[static_cast<size_t>(type)];
        }
    };

} // namespace game::stats