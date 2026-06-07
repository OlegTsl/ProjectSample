#pragma once

#include "game/components/Skills.hpp"

namespace game::skills {

    class SkillFactory {
    public:
        static components::BuffSkillComponent   makeWarriorRage();
        static components::DamageSkillComponent makeArcherPrecisionShot();
        static components::DamageSkillComponent makeMageFireblast();
    };

} // namespace game::skills