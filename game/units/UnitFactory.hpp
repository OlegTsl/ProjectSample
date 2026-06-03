#pragma once

#include "core/Context.hpp"
#include "UnitRegistry.hpp"

namespace game {
    class Unit;
    class UnitFactory {
    public:
        UnitFactory(core::Context& context, UnitRegistry& registry);

        Unit* createWarrior(int x, int y, int team);
        Unit* createArcher (int x, int y, int team);
        Unit* createMage   (int x, int y, int team);

    private:
        Unit* registerUnit(core::Entity& entity);

        core::Context& _context;
        UnitRegistry&  _registry;
    };
} // namespace game