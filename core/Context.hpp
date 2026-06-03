#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <bitset>
#include <cassert>
#include "Entity.hpp"
#include "ComponentPool.hpp"

namespace game::core {

class Context {
public:
    static constexpr size_t MAX_COMPONENTS = 128;
    
    Context();
    ~Context() = default;
    
    Context(const Context&)            = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&)                 = default;
    Context& operator=(Context&&)      = default;
    
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
        auto* pool = getPool<T>();
        if (!pool)
            return nullptr;
        
        return pool->get(entity);
    }
    
    template<typename T>
    const T* getComponent(Entity entity) const {
        auto typeIdx = std::type_index(typeid(T));
        auto it      = _pools.find(typeIdx);

        if (it == _pools.end())
            return nullptr;
        
        auto* wrapper = static_cast<const PoolWrapper<T>*>(it->second.get());
        return wrapper->pool.get(entity);
    }
    
    template<typename T>
    bool hasComponent(Entity entity) const {
        if (!isAlive(entity))
            return false;
        
        size_t bit = const_cast<Context*>(this)->getComponentBit<T>();
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
        
        pool->add(entity, std::forward<T>(component));
        
        size_t bit = getComponentBit<T>();
        _entityRecords[entity.id].componentMask.set(bit);
        
        return pool->get(entity);
    }
    
    template<typename T>
    void removeComponent(Entity entity) {
        auto* pool = getPool<T>();
        if (!pool || !pool->has(entity))
            return;
        
        pool->remove(entity);

        size_t bit = getComponentBit<T>();
        _entityRecords[entity.id].componentMask.reset(bit);
    }
    
    template<typename T>
    ComponentPool<T>* getPool() {
        auto typeIdx = std::type_index(typeid(T));
        auto it      = _pools.find(typeIdx);

        if (it != _pools.end()) {
            return &static_cast<PoolWrapper<T>*>(it->second.get())->pool;
        }
        
        auto wrapper = std::make_unique<PoolWrapper<T>>();
        auto* ptr    = &wrapper->pool;

        _pools[typeIdx] = std::move(wrapper);
        return ptr;
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

    mutable std::unordered_map<std::type_index, std::unique_ptr<IPoolBase>> _pools;
    std::unordered_map<std::type_index, size_t>                             _componentBits;
    std::unordered_map<size_t, std::type_index>                             _bitToType;
    std::vector<EntityRecord>                                               _entityRecords;
    std::vector<uint32_t>                                                   _freeEntityIds;
    size_t                                                                  _entityCount      = 0;
    size_t                                                                  _nextComponentBit = 0;
    
    Entity allocateEntity();
    
    template<typename T>
    size_t getComponentBit() {
        auto typeIdx = std::type_index(typeid(T));
        auto it      = _componentBits.find(typeIdx);

        if (it != _componentBits.end()) {
            return it->second;
        }
        
        assert(_nextComponentBit < MAX_COMPONENTS);

        size_t bit              = _nextComponentBit++;
        _componentBits[typeIdx] = bit;
        _bitToType.emplace(bit, typeIdx);

        return bit;
    }
};

} // namespace game::core