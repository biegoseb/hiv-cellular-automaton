#ifndef PREDATOR_AND_PREY_H
#define PREDATOR_AND_PREY_H

#include <vector>
#include "../CellularAutomaton.h"
#include "creature.h"
#include "../util/random.h"
#include "../resource-manager/resourceHolder.h"

using std::vector;

class PredatorAndPrey : public CellularAutomaton {
    private:
        enum Cell { On, Off };
        void updatePredator(Creature& thisCreature, Creature& otherCreature);
        void updatePrey(Creature& thisCreature, Creature& otherCreature);
        vector<Creature> creatures;
        vector<Cell> cells;
        sf::Text preyCountText;
        sf::Text predCountText;
        int preyCount = 0;
        int predatorCount = 0;
    public:
     PredatorAndPrey(const Config& cfg, const Application& app);
     void update() override;
     void onRenderGUI(sf::RenderWindow& window) override;
};

void initText(sf::Text& text, float y) {
    text.move(10, y);
    text.setFillColor(sf::Color::White);
    text.setFont(ResourceHolder::get().fonts.get("arial"));
    text.setCharacterSize(18);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
}

PredatorAndPrey::PredatorAndPrey(const Config& config, const Application& app)
    : CellularAutomaton(config, app), creatures(config.simSize.x * config.simSize.y) {
    cellForEach([&](unsigned x, unsigned y) {
        auto index = getCellIndex(x, y);
        auto type = creatures[index].getType();
        switch (type) {
        case CreatureType::prey:
            ++preyCount;
            break;
        case CreatureType::predator:
            ++predatorCount;
            break;
        case CreatureType::nothing:
            break;
        }
        sf::Color creatureColor = creatures[index].getColor();
        setCellColor(x, y, creatureColor);
    });
    initText(preyCountText, 40);
    initText(predCountText, 60);

}

void PredatorAndPrey::update() {
    cellForEach([&](unsigned x, unsigned y) {
        auto index = getCellIndex(x, y);
        auto& thisCreature = creatures[index];
        auto thisType = thisCreature.getType();

        if (thisType == CreatureType::nothing) return;

        int xChange = Random::get().intInRange(-1, 1);
        int yChange = Random::get().intInRange(-1, 1);
        int xAdj = x + xChange;
        int yAdj = y + yChange;

        if (xAdj < 0 || xAdj >= (int)myConfig->simSize.x) return;
        if (yAdj < 0 || yAdj >= (int)myConfig->simSize.y) return;

        auto adjIndex = getCellIndex(xAdj, yAdj);
        auto& otherCreature = creatures[adjIndex];

        thisCreature.update();
        switch (thisType) {
            case CreatureType::predator:
                updatePredator(thisCreature, otherCreature);
                break;
            case CreatureType::prey:
                updatePrey(thisCreature, otherCreature);
                break;
            default:
                break;
        }
        sf::Color creatureColor = creatures[index].getColor();
        setCellColor(x, y, creatureColor);
    });
}

void PredatorAndPrey::onRenderGUI(sf::RenderWindow& window) {
    preyCountText.setString("Predators: " + std::to_string(predatorCount));
    predCountText.setString("Prey: " + std::to_string(preyCount)); 
    window.draw(predCountText);
    window.draw(preyCountText);
}

void PredatorAndPrey::updatePredator(Creature& thisCreature, Creature& otherCreature) {
    if (thisCreature.getHealth() <= 0) {
        --predatorCount;
        thisCreature.setType(CreatureType::nothing);
        return;
    }

    auto otherType = otherCreature.getType();
    switch (otherType) {
        case CreatureType::prey:
            --preyCount;
            ++predatorCount;
            otherCreature.setType(CreatureType::predator);
            thisCreature.heal(otherCreature.getHealth());
            break;
        case CreatureType::predator:
            break;
        case CreatureType::nothing:
            thisCreature.move(otherCreature);
            break;
    }
}

void PredatorAndPrey::updatePrey(Creature& thisCreature, Creature& otherCreature) {
    auto otherType = otherCreature.getType();

    bool reproduce = false;
    if (thisCreature.getHealth() >= MAX_HEALTH) {
        thisCreature.setHealth(10);
        reproduce = true;
    }

    switch (otherType) {
        case CreatureType::prey:
            break;
        case CreatureType::predator:
            break;
        case CreatureType::nothing:
            if (reproduce) {
                ++preyCount;
                thisCreature.reproduce(otherCreature);
            } else {
                thisCreature.move(otherCreature);
            }
            break;
    }
}

#endif // PREDATOR_AND_PREY_H