#include "SkillFactory.hpp"

namespace game::skills {

    components::BuffSkillComponent SkillFactory::makeWarriorRage() {
        return { .skill = { .stat = stats::StatType::Strength, .multiplier = 1.5f, .duration = 3, .cooldown = 5 } };
    }

    components::DamageSkillComponent SkillFactory::makeArcherPrecisionShot() {
        return { .skill = { .stat = stats::StatType::Agility, .multiplier = 2.0f, .cooldown = 3 } };
    }

    components::DamageSkillComponent SkillFactory::makeMageFireblast() {
        return { .skill = { .stat = stats::StatType::Intellect, .multiplier = 3.0f, .cooldown = 4 } };
    }

} // namespace game::skills