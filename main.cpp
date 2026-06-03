#include <iostream>
#include <string>

#include "core/Context.hpp"
#include "components/Attributes.hpp"
#include "components/Resources.hpp"
#include "components/Transform.hpp"
#include "components/Identity.hpp"

int main() {
    using namespace game::core;
    using namespace game::components;

    Context context;

    auto warrior = context.createEntity(
        Health{120, 120},
        Strength{15},
        Position{2, 7},
        Name{"Aragorn"}
    );

    auto mage = context.createEntity(
        Health{60, 60},
        Mana{100, 100},
        Position{0, 5},
        Name{"Gandalf"}
    );

    auto archer = context.createEntity(
        Health{80, 80},
        Agility{20},
        Position{5, 0},
        Name{"Legolas"}
    );


    std::string command;
        
    if (auto* health = context.getComponent<Health>(warrior)) {
        std::cout << "Warrior HP: " << health->current << "/" << health->max << std::endl;
    }
    
    // Получить имя мага
    if (auto* name = context.getComponent<Name>(mage)) {
        std::cout << "Mage name: " << name->value << std::endl;
    }
    
    // Изменить позицию лучника
    if (auto* pos = context.getComponent<Position>(archer)) {
        pos->x += 10.0f;
        pos->y += 3.0f;
        std::cout << "Archer moved to: " << pos->x << ", " << pos->y << std::endl;
    }
    
    // === Проверка наличия компонента ===
    
    std::cout << "Warrior has Mana: " << context.hasComponent<Mana>(warrior) << std::endl;
    std::cout << "Mage has Mana: " << context.hasComponent<Mana>(mage) << std::endl;
    std::cout << "Archer has Strength: " << context.hasComponent<Strength>(archer) << std::endl;
    
    // === Добавление компонента существующей сущности ===
    
    // Лучник подобрал меч — добавляем силу
    context.addComponent<Strength>(archer, Strength{8});
    std::cout << "Archer now has Strength: " << context.hasComponent<Strength>(archer) << std::endl;
    
    if (auto* str = context.getComponent<Strength>(archer)) {
        std::cout << "Archer strength: " << str->value << std::endl;
    }
    
    // === Удаление компонента ===
    
    // Маг истратил всю ману — убираем компонент
    context.removeComponent<Mana>(mage);
    std::cout << "Mage has Mana after remove: " << context.hasComponent<Mana>(mage) << std::endl;
    
    // === Удаление сущности ===
    
    std::cout << "Warrior alive: " << context.isAlive(warrior) << std::endl;
    context.destroyEntity(warrior);
    std::cout << "Warrior alive after destroy: " << context.isAlive(warrior) << std::endl;
    
    // Попытка получить компонент умершей сущности
    if (auto* health = context.getComponent<Health>(warrior)) {
        std::cout << "This won't print" << std::endl;
    } else {
        std::cout << "Dead entity has no components" << std::endl;
    }
    
    // === Статистика ===
    
    std::cout << "Total entities: " << context.getEntityCount() << std::endl;
    
    return 0;
    tobqId-fyfgyn-huvto9
}