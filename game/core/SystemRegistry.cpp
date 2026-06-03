#include "SystemRegistry.hpp"

namespace game::core {

    void SystemRegistry::initializeAll(Context& ctx) {
        for (auto& system : _initSystems) {
            system->initialize(ctx);
        }
    }

    void SystemRegistry::updateAll(Context& ctx, float dt) {
        for (auto& system : _updateSystems) {
            system->update(ctx, dt);
        }
    }

    void SystemRegistry::clear() {
        _initSystems.clear();
        _updateSystems.clear();
    }

} // namespace game::core