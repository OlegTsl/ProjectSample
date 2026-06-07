#pragma once

#include "Entity.hpp"

#include <vector>
#include <cassert>
#include <limits>

namespace game::core {

    template<typename T>
    class ComponentPool {
    public:
        static constexpr size_t INVALID = std::numeric_limits<size_t>::max();

        T* add(Entity entity, T&& component) {
            assert(!has(entity));

            ensureCapacity(entity.id);

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
            _entityToIndex[entity.id] = INVALID;
        }

        T* get(Entity entity) {
            if (entity.id >= _entityToIndex.size())
                return nullptr;

            size_t index = _entityToIndex[entity.id];
            if (index == INVALID)
                return nullptr;

            return &_data[index];
        }

        const T* get(Entity entity) const {
            if (entity.id >= _entityToIndex.size())
                return nullptr;

            size_t index = _entityToIndex[entity.id];
            if (index == INVALID)
                return nullptr;

            return &_data[index];
        }

        bool has(Entity entity) const {
            if (entity.id >= _entityToIndex.size())
                return false;

            return _entityToIndex[entity.id] != INVALID;
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
        std::vector<T>      _data;
        std::vector<Entity> _entities;
        std::vector<size_t> _entityToIndex;

        void ensureCapacity(uint32_t id) {
            if (id >= _entityToIndex.size())
                _entityToIndex.resize(id + 1, INVALID);
        }
    };

} // namespace game::core