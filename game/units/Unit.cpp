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

    components::UnitType* Unit::unitType() {
        return _ctx.getComponent<components::UnitType>(_entity);
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

    components::Damage* Unit::damage() {
        return _ctx.getComponent<components::Damage>(_entity);
    }

    components::Position* Unit::position() {
        return _ctx.getComponent<components::Position>(_entity);
    }

    components::Team* Unit::team() {
        return _ctx.getComponent<components::Team>(_entity);
    }

    components::UnitType* Unit::type() {
        return _ctx.getComponent<components::UnitType>(_entity);
    }

    bool Unit::isAlive() const {
        return _ctx.isAlive(_entity);
    }

    components::UnitClass Unit::getType() const {
        auto* type = _ctx.getComponent<components::UnitType>(_entity);
        return type ? type->type : components::UnitClass::Warrior;
    }

    int Unit::getTeam() const {
        auto* team = const_cast<Unit*>(this)->team();
        return team ? team->value : 0;
    }
}
