#pragma once

#include "core/Entity.hpp"
#include "core/Context.hpp"
#include "components/Identity.hpp"

namespace game::components {
    struct Health;
    struct Mana;
    struct Strength;
    struct Agility;
    struct Intellect;
    struct BaseDamage;
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
        components::Strength*   strength();
        components::Agility*    agility();
        components::Intellect*  intellect();
        components::BaseDamage* baseDamage();
        components::Position*   position();
        components::Team*       team();

        bool isAlive() const;
        int  getTeam() const;

    private:
        core::Entity   _entity;
        core::Context& _ctx;
    };

} // namespace game