#include "SystemRegistry.hpp"

namespace game::core {

    void SystemRegistry::initializeAll(Context& context) const {
        for (auto& system : _initSystems) {
            system->initialize(context);
        }
    }

    void SystemRegistry::updateAll(Context& context, float dt) const {
        for (auto& system : _updateSystems) {
            system->update(context, dt);
        }
    }

    void SystemRegistry::clear() {
        _initSystems.clear();
        _updateSystems.clear();
    }

} // namespace game::core