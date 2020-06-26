#include <vector>
#include <iostream>
#include <random>
#include "../CellularAutomaton.h"

class GameOfLife : public CellularAutomaton {
    private:
        enum Cell { On,Off };
        std::vector<Cell> myCells;
    public:
        GameOfLife(const Config& config, const Application& app);
        void update() override;    
};

GameOfLife::GameOfLife(const Config & config, const Application& app)
    : CellularAutomaton(config, app), myCells(config.simSize.x * config.simSize.y) {
    std::mt19937 range((unsigned)std::time(nullptr));
    cellForEach([&](unsigned x, unsigned y) {
        unsigned index = getCellIndex(x, y);
        std::uniform_int_distribution<int> dist(0, 1);
        auto& cell = myCells[index];
        cell = (Cell)dist(range);
        CellularAutomaton::setCellColor(x, y, cell == Cell::On ? sf::Color::Black : myConfig->fgColor);
    });
}

void GameOfLife::update() {
    std::vector<std::pair<sf::Vector2i, Cell>> updates;
    cellForEach([&](unsigned x, unsigned y) {
        sf::Vector2i loc(x, y);
        unsigned count = 0;
        for (int nX = -1; nX <= 1; ++nX) { //check neighbours
            for (int nY = -1; nY <= 1; ++nY) {
                int newX = nX + x;
                int newY = nY + y;
                if (newX == -1 || newX == (int)myConfig->simSize.x ||
                    newY == -1 || newY == (int)myConfig->simSize.y || //out of bounds
                    (nX == 0 && nY == 0)) //Cell itself
                {
                    continue;
                }
                auto cell = myCells[getCellIndex(newX, newY)];
                if (cell == Cell::On) {
                    ++count;
                }
            }
        }
        int index = getCellIndex(x, y);
        auto cell = myCells[index];
        switch (cell) {
            case Cell::On:
                if (count < 2 || count > 3) {
                    updates.emplace_back(loc, Cell::Off);
                }
                break;
            case Cell::Off: /* Dead cell with 3 living neighbors, get reborn */
                if (count == 3) {
                    updates.emplace_back(loc, Cell::On);
                }
                break;
        } 
    });
    for (auto& update : updates) {
        myCells[getCellIndex(update.first.x, update.first.y)] = update.second;
        CellularAutomaton::setCellColor(update.first.x, update.first.y, update.second == Cell::On ? myConfig->bgColor : myConfig->fgColor);
    }
}