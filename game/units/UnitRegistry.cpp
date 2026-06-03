#include "UnitRegistry.hpp"
#include "Unit.hpp"

namespace game {
    void UnitRegistry::registerUnit(std::unique_ptr<Unit> unit) {
        Unit* ptr = unit.get();
        _units.push_back(std::move(unit));
        _entityToUnit[ptr->getEntity().id] = ptr;
        addToIndices(ptr);
    }

    void UnitRegistry::unregisterUnit(core::Entity entity) {
        auto it = _entityToUnit.find(entity.id);
        if (it == _entityToUnit.end()) return;

        Unit* unit = it->second;
        removeFromIndices(unit);
        _entityToUnit.erase(it);

        auto unitIt = std::find_if(_units.begin(), _units.end(),
            [unit](const auto& u) { return u.get() == unit; });
        if (unitIt != _units.end()) {
            _units.erase(unitIt);
        }
    }

    Unit* UnitRegistry::getUnit(core::Entity entity) const {
        auto it = _entityToUnit.find(entity.id);
        return it != _entityToUnit.end() ? it->second : nullptr;
    }

    const std::vector<Unit*>& UnitRegistry::getAll() const {
        return _all;
    }

    const std::vector<Unit*>& UnitRegistry::getByType(components::UnitClass type) const {
        static std::vector<Unit*> empty;
        auto it = _typed.find(type);
        return it != _typed.end() ? it->second : empty;
    }

    const std::vector<Unit*>& UnitRegistry::getByTeam(int team) const {
        static std::vector<Unit*> empty;
        auto it = _teamed.find(team);
        return it != _teamed.end() ? it->second : empty;
    }

    const std::vector<Unit*>& UnitRegistry::getEnemies(int team) const {
        static std::vector<Unit*> enemies;
        enemies.clear();

        for (auto& [t, units] : _teamed) {
            if (t != team) {
                enemies.insert(enemies.end(), units.begin(), units.end());
            }
        }

        return enemies;
    }

    void UnitRegistry::addToIndices(Unit* unit) {
        _all.push_back(unit);
        _typed[unit->getType()].push_back(unit);
        _teamed[unit->getTeam()].push_back(unit);
    }

    void UnitRegistry::removeFromIndices(Unit* unit) {
        _all.erase(std::remove(_all.begin(), _all.end(), unit), _all.end());

        auto typeIt = _typed.find(unit->getType());
        if (typeIt != _typed.end()) {
            auto& vec = typeIt->second;
            vec.erase(std::remove(vec.begin(), vec.end(), unit), vec.end());
        }

        auto teamIt = _teamed.find(unit->getTeam());
        if (teamIt != _teamed.end()) {
            auto& vec = teamIt->second;
            vec.erase(std::remove(vec.begin(), vec.end(), unit), vec.end());
        }
    }
} // namespace game