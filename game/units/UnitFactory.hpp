#pragma once

#include "UnitRegistry.hpp"
#include "game/tools/Vec2.hpp"
#include "game/core/Context.hpp"

namespace game {
    class Unit;
    class UnitFactory {
    public:
        UnitFactory(core::Context& context, UnitRegistry& registry);

        Unit* createWarrior(tools::Vec2 pos, int team);
        Unit* createArcher (tools::Vec2 pos, int team);
        Unit* createMage   (tools::Vec2 pos, int team);

    private:
        Unit* registerUnit(core::Entity& entity);

        core::Context& _context;
        UnitRegistry&  _registry;
    };
} // namespace game