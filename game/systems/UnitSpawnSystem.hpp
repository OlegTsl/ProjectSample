#pragma once

#include "game/core/SystemRegistry.hpp"
#include "game/units/UnitFactory.hpp"

namespace game::systems {

    class UnitSpawnSystem : public core::InitializeSystem {
    public:
        UnitSpawnSystem(UnitFactory& factory);
        void initialize(core::Context& ctx) override;

    private:
        UnitFactory& _factory;
    };

} // namespace game::systems