#ifndef PREDATOR_AND_PREY_H
#define PREDATOR_AND_PREY_H

#include <vector>
#include "../CellularAutomaton.h"
#include "creature.h"

using std::vector;

class PredatorAndPrey : public CellularAutomaton {
    private:
        enum Cell { On, Off };
        void updatePredator(Creature& thisCreature, Creature& otherCreature);
        void updatePrey(Creature& thisCreature, Creature& otherCreature);
        vector<Creature> creatures;
        vector<Cell> cells;
        sf::Text preyCountText;
        sf::Text predatorCountText;
        int preyCount = 0;
        int predatorCount = 0;
    public:
     PredatorAndPrey(const Config& cfg, const Application& app);
     void update() override;
     void onRenderGUI(sf::RenderWindow& window) override;
};

#endif // PREDATOR_AND_PREY_H