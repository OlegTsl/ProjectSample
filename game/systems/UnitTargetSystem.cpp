#include "UnitTargetSystem.hpp"
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

            auto nearest = getTarget(entity, *position, *team, *positions, *teams);
            if (nearest)
                context.addComponent<components::Target>(entity, {*nearest});
            else
                context.removeComponent<components::Target>(entity);
        }
    }

    std::optional<core::Entity> UnitTargetSystem::getTarget(
        core::Entity                               entity,
        const components::Position&                position,
        const components::Team&                    team,
        core::ComponentPool<components::Position>& positions,
        core::ComponentPool<components::Team>&     teams)
    {
        core::Entity bestEntity{};
        int          bestDist = std::numeric_limits<int>::max();
        bool         found    = false;

        for (auto candidate : positions.entities()) {
            if (candidate == entity)
                continue;

            auto* candidateTeam = teams.get(candidate);
            if (!candidateTeam || candidateTeam->value == team.value)
                continue;

            auto* candidatePos = positions.get(candidate);
            if (!candidatePos)
                continue;

            int dist = position.value.distance(candidatePos->value);
            if (dist < bestDist) {
                bestDist   = dist;
                bestEntity = candidate;
                found      = true;
            }
        }

        return found ? std::optional{bestEntity} : std::nullopt;
    }

} // namespace game::systems