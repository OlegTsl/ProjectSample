#pragma once

#include "game/core/Entity.hpp"
#include "game/stats/StatType.hpp"

namespace game::components
{
    struct BaseDamage   { int value; };
    struct AttackRange  { int value; };
    struct MeleeAttack  { stats::StatType stat; };
    struct RangedAttack { stats::StatType stat; };
    struct Target       { core::Entity entity; };
}