#include "Context.hpp"
#include <cassert>

namespace game::core {

static constexpr size_t MAX_RECORDS   = 1024;
static constexpr size_t MAX_POOL_SIZE = 256;

Context::Context() {
    _entityRecords.reserve(MAX_RECORDS);
    _freeEntityIds.reserve(MAX_POOL_SIZE);
}

void Context::onDestroy(DestroyCallback callback) {
    _destroyCallbacks.push_back(std::move(callback));
}

Entity Context::allocateEntity() {
    Entity entity;
    
    if (!_freeEntityIds.empty()) {
        entity.id         = _freeEntityIds.back();
        entity.generation = ++_entityRecords[entity.id].generation;

        _freeEntityIds.pop_back();
    } else {
        entity.id         = static_cast<uint32_t>(_entityRecords.size());
        entity.generation = 1;

        _entityRecords.emplace_back();
        _entityRecords.back().generation = 1;
    }
    
    _entityCount++;
    return entity;
}

void Context::destroyEntity(Entity entity) {
    assert(isAlive(entity));

    for (auto& callback : _destroyCallbacks) {
        callback(entity);
    }
    
    auto& [componentMask, generation] = _entityRecords[entity.id];

    for (size_t bit = 0; bit < MAX_COMPONENTS; ++bit) {
        if (!componentMask.test(bit))
            continue;

        if (bit < _pools.size() && _pools[bit]) {
            _pools[bit]->removeFromEntity(entity);
        }
    }
    
    componentMask.reset();
    generation++;
    
    _freeEntityIds.push_back(entity.id);
    _entityCount--;
}

bool Context::isAlive(Entity entity) const {
    if (entity.id >= _entityRecords.size())
        return false;

    return _entityRecords[entity.id].generation == entity.generation
        && entity.generation > 0;
}

size_t Context::getEntityCount() const {
    return _entityCount;
}

} // namespace game::core