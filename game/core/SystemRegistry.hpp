#pragma once

#include <vector>
#include <memory>
#include "game/core/Context.hpp"

namespace game::core {

    class InitializeSystem {
    public:
        virtual ~InitializeSystem() = default;
        virtual void initialize(Context& context) = 0;
    };

    class UpdateSystem {
    public:
        virtual ~UpdateSystem() = default;
        virtual void update(Context& context, float dt) = 0;
    };

    class SystemRegistry {
    public:
        SystemRegistry()  = default;
        ~SystemRegistry() = default;

        template<typename... Systems>
        void addInitialize(std::unique_ptr<Systems>... systems) {
                (_initSystems.push_back(std::move(systems)), ...);
            }

        template<typename... Systems>
        void addUpdate(std::unique_ptr<Systems>... systems) {
            (_updateSystems.push_back(std::move(systems)), ...);
        }

        void initializeAll(Context& context)       const;
        void updateAll(Context& context, float dt) const;

        void clear();

    private:
        std::vector<std::unique_ptr<InitializeSystem>> _initSystems;
        std::vector<std::unique_ptr<UpdateSystem>>     _updateSystems;
    };

} // namespace game::core