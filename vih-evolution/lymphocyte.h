#ifndef LYMPHOCYTE_H
#define LYMPHOCYTE_H

#include <SFML/Graphics.hpp>
#include "../util/random.h"

/* Lymphocyte T CD4+ */
enum LymphocyteStatus {
    healthy,    // green
    identified, // blue
    latent,     // yellow
    infected,   // red
    dead        // black
};

class Lymphocyte {
private:
    LymphocyteStatus status;
    float latentProb;
    float deadProb;
public:
    Lymphocyte();
    sf::Color getColor();
    LymphocyteStatus getStatus() const;
    int getStatusNumber() const;
    void setStatus(LymphocyteStatus status);
    float getLatentProb() const;
    float getDeadProb() const;
};

Lymphocyte::Lymphocyte() {
    int n = Random::get().intInRange(0, 1000);
    if (n > 200)        status = LymphocyteStatus::healthy;
    else if (n > 150)   status = LymphocyteStatus::identified;
    else if (n > 100)   status = LymphocyteStatus::infected;
    else                status = LymphocyteStatus::dead;

    latentProb = Random::get().floatInRange(0, 1);
    deadProb = Random::get().floatInRange(0, 1);
}

sf::Color Lymphocyte::getColor() {
    switch (status) {
    case LymphocyteStatus::healthy:
        return sf::Color::Green;
        break;
    case LymphocyteStatus::identified:
        return sf::Color::Blue;
        break;
    case LymphocyteStatus::latent:
        return sf::Color::Yellow;
        break;
    case LymphocyteStatus::infected:
        return sf::Color::Red;
        break;
    case LymphocyteStatus::dead:
        return sf::Color::Black;
        break;
    default:
        return sf::Color::Black;
        break;
    }
}

LymphocyteStatus Lymphocyte::getStatus() const {
    return status;
}

int Lymphocyte::getStatusNumber() const {
    switch (status) {
    case LymphocyteStatus::healthy:
        return 0;
        break;
    case LymphocyteStatus::identified:
        return 1;
        break;
    case LymphocyteStatus::latent:
        return 2;
        break;
    case LymphocyteStatus::infected:
        return 3;
        break;
    case LymphocyteStatus::dead:
        return 4;
        break;
    default:
        return 4;
        break;
    }
}

void Lymphocyte::setStatus(LymphocyteStatus status) {
    this->status = status;
}

float Lymphocyte::getLatentProb() const {
    return latentProb;
}

float Lymphocyte::getDeadProb() const {
    return deadProb;
}

#endif // LYMPHOCYTE_H