#pragma once

#include "game/core/SystemRegistry.hpp"
#include "game/units/UnitFactory.hpp"
#include "game/grid/Grid.hpp"

namespace game::systems {

    class UnitSpawnSystem : public core::InitializeSystem {
    public:
        UnitSpawnSystem(UnitFactory& factory, Grid& grid);
        void initialize(core::Context& context) override;

    private:
        UnitFactory& _factory;
        Grid&        _grid;
    };

} // namespace game::systems