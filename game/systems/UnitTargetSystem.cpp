#include "UnitTargetSystem.hpp"
#include "components/Identity.hpp"
#include "components/Transform.hpp"
#include "components/Combat.hpp"

namespace game::systems {

    UnitTargetSystem::UnitTargetSystem(const Grid& grid)
        : _grid(grid)
    {}

    void UnitTargetSystem::update(core::Context& context, float dt) {
        auto* positions = context.getPool<components::Position>();
        auto* teams     = context.getPool<components::Team>();

        if (!positions || !teams)
            return;

        for (auto entity : positions->entities()) {
            auto* position = positions->get(entity);
            auto* team     = teams->get(entity);

            if (!position || !team)
                continue;

            core::Entity bestEntity{};
            int          bestDist  = std::numeric_limits<int>::max();
            bool         found     = false;

            for (auto candidate : positions->entities()) {
                if (candidate == entity)
                    continue;

                auto* candidateTeam = teams->get(candidate);
                if (!candidateTeam || candidateTeam->value == team->value)
                    continue;

                auto* candidatePos = positions->get(candidate);
                if (!candidatePos)
                    continue;

                int dist = position->value.distance(candidatePos->value);
                if (dist < bestDist) {
                    bestDist   = dist;
                    bestEntity = candidate;
                    found      = true;
                }
            }

            if (found) {
                context.addComponent<components::Target>(entity, { bestEntity });
            } else {
                context.removeComponent<components::Target>(entity);
            }
        }
    }

} // namespace game::systems