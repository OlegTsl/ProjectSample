#include "Unit.hpp"

#include "components/Attributes.hpp"
#include "components/Resources.hpp"
#include "components/Combat.hpp"
#include "components/Transform.hpp"

namespace game {

    Unit::Unit(core::Entity entity, core::Context& ctx)
        : _entity(entity)
        , _ctx(ctx)
    {
    }

    core::Entity Unit::getEntity() const {
        return _entity;
    }

    components::Health* Unit::health() {
        return _ctx.getComponent<components::Health>(_entity);
    }

    components::Mana* Unit::mana() {
        return _ctx.getComponent<components::Mana>(_entity);
    }

    components::Strength* Unit::strength() {
        return _ctx.getComponent<components::Strength>(_entity);
    }

    components::Agility* Unit::agility() {
        return _ctx.getComponent<components::Agility>(_entity);
    }

    components::Intellect* Unit::intellect() {
        return _ctx.getComponent<components::Intellect>(_entity);
    }

    components::BaseDamage* Unit::baseDamage() {
        return _ctx.getComponent<components::BaseDamage>(_entity);
    }

    components::Position* Unit::position() {
        return _ctx.getComponent<components::Position>(_entity);
    }

    components::Team* Unit::team() {
        return _ctx.getComponent<components::Team>(_entity);
    }

    bool Unit::isAlive() const {
        return _ctx.isAlive(_entity);
    }

    int Unit::getTeam() const {
        auto* team = _ctx.getComponent<components::Team>(_entity);
        return team ? team->value : 0;
    }
}
