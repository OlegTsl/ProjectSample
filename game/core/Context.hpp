#pragma once

#include <vector>
#include <memory>
#include <bitset>
#include <cassert>
#include <functional>

#include "Entity.hpp"
#include "ComponentPool.hpp"

namespace game::core {

inline size_t& componentIdCounter() {
    static size_t counter = 0;
    return counter;
}

template<typename T>
size_t componentId() {
    static size_t id = componentIdCounter()++;
    return id;
}

class Context {
public:
    static constexpr size_t MAX_COMPONENTS = 128;
    
    Context();
    ~Context() = default;
    
    Context(const Context&)            = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&)                 = default;
    Context& operator=(Context&&)      = default;

    using DestroyCallback = std::function<void(Entity)>;
    void onDestroy(DestroyCallback callback);
    
    template<typename... Components>
    Entity createEntity(Components&&... components) {
        Entity entity = allocateEntity();
        (addComponent<std::decay_t<Components>>(entity, std::forward<Components>(components)), ...);
        return entity;
    }
    
    void destroyEntity(Entity entity);
    bool isAlive(Entity entity) const;
    
    template<typename T>
    T* getComponent(Entity entity) {
        size_t id = componentId<T>();
        if (id >= _pools.size() || !_pools[id])
            return nullptr;

        auto* wrapper = static_cast<PoolWrapper<T>*>(_pools[id].get());
        return wrapper->pool.get(entity);
    }
    
    template<typename T>
    const T* getComponent(Entity entity) const {
        size_t id = componentId<T>();
        if (id >= _pools.size() || !_pools[id])
            return nullptr;

        auto* wrapper = static_cast<const PoolWrapper<T>*>(_pools[id].get());
        return wrapper->pool.get(entity);
    }
    
    template<typename T>
    bool hasComponent(Entity entity) const {
        if (!isAlive(entity))
            return false;
        
        size_t bit = componentId<T>();
        return _entityRecords[entity.id].componentMask.test(bit);
    }
    
    template<typename T>
    T* addComponent(Entity entity, T&& component) {
        auto* pool = getPool<T>();
        
        if (pool->has(entity)) {
            T* existing = pool->get(entity);
            *existing   = std::forward<T>(component);
            return existing;
        }

        size_t bit = componentId<T>();
        _entityRecords[entity.id].componentMask.set(bit);
        
        return pool->add(entity, std::forward<T>(component));
    }
    
    template<typename T>
    void removeComponent(Entity entity) {
        size_t id = componentId<T>();
        if (id >= _pools.size() || !_pools[id]) return;

        auto* pool = &static_cast<PoolWrapper<T>*>(_pools[id].get())->pool;
        if (!pool->has(entity)) return;

        pool->remove(entity);
        _entityRecords[entity.id].componentMask.reset(id);
    }

    template<typename T>
    ComponentPool<T>* getPool() {
        size_t id = componentId<T>();
        if (id >= _pools.size()) {
            _pools.resize(id + 1);
        }
        if (!_pools[id]) {
            _pools[id] = std::make_unique<PoolWrapper<T>>();
        }
        return &static_cast<PoolWrapper<T>*>(_pools[id].get())->pool;
    }

    size_t getEntityCount() const;
    
private:
    struct EntityRecord {
        std::bitset<MAX_COMPONENTS> componentMask;
        uint32_t                    generation    = 0;
    };
    
    struct IPoolBase {
        virtual ~IPoolBase()                         = default;
        virtual void removeFromEntity(Entity entity) = 0;
    };
    
    template<typename T>
    struct PoolWrapper : IPoolBase {
        ComponentPool<T> pool;
        
        void removeFromEntity(Entity entity) override {
            pool.remove(entity);
        }
    };

    std::vector<std::unique_ptr<IPoolBase>> _pools;
    std::vector<EntityRecord>               _entityRecords;
    std::vector<uint32_t>                   _freeEntityIds;
    std::vector<DestroyCallback>            _destroyCallbacks;
    size_t                                  _entityCount = 0;
    
    Entity allocateEntity();
};

} // namespace game::core