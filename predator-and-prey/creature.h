#ifndef CREATURE_H
#define CREATURE_H

#include <SFML/Graphics.hpp>
#include "../util/random.h"

const int MAX_HEALTH = 100;

enum class CreatureType {
    predator = 0,
    prey = 1,
    nothing = 2
};

class Creature {
    private:
        CreatureType type;
        int health = MAX_HEALTH / 5;
    public:
        Creature();
        sf::Color getColor();
        CreatureType getType() const;
        void setType(CreatureType type);
        void heal(int amount);
        int getHealth() const;
        void setHealth(int health);
        void update();
        void reproduce(Creature& otherCreature);
        void move(Creature& otherCreature);
};

Creature::Creature() {
    int number = Random::get().intInRange(0, 1000);
    if (number > 100) {
        type = CreatureType::nothing;
    } else if (number > 50) {
        type = CreatureType::prey;
    } else {
        type = CreatureType::predator;
    }
}

sf::Color Creature::getColor() {
    if (type == CreatureType::nothing || health == 0) {
        return sf::Color::Black;
    } else {
        float normalisedHealth = (float)health / (float)MAX_HEALTH;
        uint8_t color = uint8_t(normalisedHealth * 255); // unsigned int de 8 bits
        switch (type) {
        case CreatureType::predator:
            return {color, 0, 0};
        case CreatureType::prey:
            return {0, color, 0};   
        default:
            return sf::Color::Black;
        }
    }
}

CreatureType Creature::getType() const {
    return type;
}

void Creature::setType(CreatureType type) {
    this->type = type;
}

void Creature::heal(int amount) {
    health += amount;
    health = std::min(health, MAX_HEALTH); // max health limit
}

int Creature::getHealth() const {
    return health;
}

void Creature::setHealth(int health) {
    this->health = health;
}

void Creature::update() {
    switch (type) {
    case CreatureType::predator:
        heal(-1);
        break;
    case CreatureType::prey:
        heal(1);
        break;
    default:
        break;
    }
}

void Creature::reproduce(Creature& otherCreature) {
    otherCreature.health = 10;
    otherCreature.type = CreatureType::prey;
}

void Creature::move(Creature& otherCreature) {
    otherCreature.health = health;
    otherCreature.type = type;
    type = CreatureType::nothing;
}

#endif // CREATURE_H