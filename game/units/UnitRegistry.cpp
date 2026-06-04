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
            std::iter_swap(unitIt, _units.end() - 1);
            _units.pop_back();
        }
    }

    Unit* UnitRegistry::getUnit(core::Entity entity) const {
        auto it = _entityToUnit.find(entity.id);
        return it != _entityToUnit.end() ? it->second : nullptr;
    }

    const std::vector<Unit*>& UnitRegistry::getAll() const {
        return _all;
    }

    const std::vector<Unit*>& UnitRegistry::getByTeam(int team) const {
        static std::vector<Unit*> empty;
        auto it = _teamed.find(team);
        return it != _teamed.end() ? it->second : empty;
    }

    std::vector<Unit*> UnitRegistry::getEnemies(int team) const {
        std::vector<Unit*> enemies;
        for (auto& [t, units] : _teamed) {
            if (t != team) {
                enemies.insert(enemies.end(), units.begin(), units.end());
            }
        }
        return enemies;
    }

    void UnitRegistry::addToIndices(Unit* unit) {
        _all.push_back(unit);
        _teamed[unit->getTeam()].push_back(unit);
    }

    void UnitRegistry::removeFromIndices(Unit* unit) {
        auto swapAndPop = [](std::vector<Unit*>& vec, Unit* u) {
            auto it = std::find(vec.begin(), vec.end(), u);
            if (it != vec.end()) {
                std::iter_swap(it, vec.end() - 1);
                vec.pop_back();
            }
        };

        swapAndPop(_all, unit);

        if (auto it = _teamed.find(unit->getTeam()); it != _teamed.end())
            swapAndPop(it->second, unit);
    }

} // namespace game