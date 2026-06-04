#pragma once

#include <cstdint>

namespace game::core
{
    struct Entity
    {
        uint32_t id         = 0;
        uint32_t generation = 0;
        
        bool operator==(const Entity& other) const {
            return id == other.id && generation == other.generation;
        }
        
        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }
    };
}