#pragma once

#include "game/core/Entity.hpp"
#include "game/core/Context.hpp"
#include "game/components/Attributes.hpp"
#include "game/components/Identity.hpp"

namespace game::components {
    struct Health;
    struct Mana;
    struct Strength;
    struct Agility;
    struct Intellect;
    struct Position;
    struct Team;
}

namespace game {
    class Unit {
    public:
        Unit(core::Entity entity, core::Context& ctx);

        core::Entity getEntity() const;

        components::Health*     health();
        components::Mana*       mana();
        components::BaseStats*  stats();
        components::Position*   position();
        components::Team*       team();

        bool isAlive() const;
        int  getTeam() const;

    private:
        core::Entity   _entity;
        core::Context& _ctx;
    };

} // namespace game