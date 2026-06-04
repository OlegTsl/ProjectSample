#pragma once

namespace game::components
{
    struct BaseDamage  { int value; };
    struct AttackRange { int value; };
    struct Target      { core::Entity entity; };
}