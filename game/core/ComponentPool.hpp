#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>
#include "Entity.hpp"

namespace game::core {

template<typename T>
class ComponentPool {
public:
    T* add(Entity entity, T&& component) {
        assert(!has(entity));
        
        size_t index = _data.size();
        _data.push_back(std::forward<T>(component));
        _entities.push_back(entity);
        _entityToIndex[entity.id] = index;

        return &_data.back();
    }
    
    void remove(Entity entity) {
        assert(has(entity));
        
        size_t index     = _entityToIndex[entity.id];
        size_t lastIndex = _data.size() - 1;
        
        if (index != lastIndex) {
            std::swap(_data[index],     _data[lastIndex]);
            std::swap(_entities[index], _entities[lastIndex]);
            _entityToIndex[_entities[index].id] = index;
        }
        
        _data.pop_back();
        _entities.pop_back();
        _entityToIndex.erase(entity.id);
    }
    
    T* get(Entity entity) {
        auto it = _entityToIndex.find(entity.id);
        if (it != _entityToIndex.end()) {
            return &_data[it->second];
        }
        return nullptr;
    }
    
    const T* get(Entity entity) const {
        auto it = _entityToIndex.find(entity.id);
        if (it != _entityToIndex.end()) {
            return &_data[it->second];
        }
        return nullptr;
    }
    
    bool has(Entity entity) const {
        return _entityToIndex.find(entity.id) != _entityToIndex.end();
    }
    
    size_t size() const {
        return _data.size();
    }
    
    const std::vector<T>& data() const {
        return _data;
    }
    
    const std::vector<Entity>& entities() const {
        return _entities;
    }
    
private:
    std::vector<T>                       _data;
    std::vector<Entity>                  _entities;
    std::unordered_map<uint32_t, size_t> _entityToIndex;
};

} // namespace game::core