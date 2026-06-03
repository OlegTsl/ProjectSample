#pragma once

#include <string>

namespace game::components
{
    enum class UnitClass : uint32_t { Warrior = 0, Archer, Mage };

    struct Name     { std::string value; };
    struct Team     { int value; };
    struct UnitType { UnitClass type; };
}