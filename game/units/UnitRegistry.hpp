#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "game/core/Entity.hpp"
#include "game/units/Unit.hpp"

namespace game {

    class UnitRegistry {
    public:
        UnitRegistry()  = default;
        ~UnitRegistry() = default;

        void registerUnit  (std::unique_ptr<Unit> unit);
        void unregisterUnit(core::Entity entity);

        Unit* getUnit(core::Entity entity) const;

        const std::vector<Unit*>& getAll()             const;
        const std::vector<Unit*>& getByTeam (int team) const;
        std::vector<Unit*> getEnemies(int team)        const;

    private:
        std::vector<std::unique_ptr<Unit>>  _units;
        std::unordered_map<uint32_t, Unit*> _entityToUnit;

        std::vector<Unit*>                          _all;
        std::unordered_map<int, std::vector<Unit*>> _teamed;

        void addToIndices     (Unit* unit);
        void removeFromIndices(Unit* unit);
    };

} // namespace game