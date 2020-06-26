#ifndef CELLULAR_AUTOMATON_H
#define CELLULAR_AUTOMATON_H

#include <vector>
#include "util/config.h"
#include "native/unix.h"

class Application;
class CellularAutomaton {
    protected:
        unsigned getCellIndex(unsigned x, unsigned y);
        void setCellColor(int x, int y, sf::Color color);     
        template <typename Func> void cellForEach(Func function);
        const Application* const myApp;
        const Config* const myConfig;
        void addQuad(unsigned x, unsigned y);
        std::vector<sf::Vertex> cellVertexPoints;
    public:
        CellularAutomaton(const Config& config, const Application& app);
        virtual ~CellularAutomaton() = default;
        virtual void input(const sf::Event& e) {}
        virtual void update() = 0;
        virtual void onRenderGUI(sf::RenderWindow& window) {}
        virtual void onRenderCells(sf::RenderWindow& window) {}
        void render(sf::RenderWindow& window);
        //void makeScreenshot();
};

/* Protected methods */
unsigned CellularAutomaton::getCellIndex(unsigned x, unsigned y) {
    return x + y * myConfig->simSize.x;
}

void CellularAutomaton::setCellColor(int x, int y, sf::Color color) {
    auto index = (y * myConfig->simSize.x + x) * 4;
    for (int i = 0; i < 4; ++i) {
        cellVertexPoints[index + i].color = color;
    }
}

template <typename Func>
void CellularAutomaton::cellForEach(Func function) {
    for (unsigned y = 0; y < myConfig->simSize.y; ++y) {
        for (unsigned x = 0; x < myConfig->simSize.x; ++x) {
            function(x, y);
        }
    }
}

void CellularAutomaton::addQuad(unsigned x, unsigned y) {
    float cellSize = (float)myConfig->cellSize;
    sf::Vertex topLeft;
    sf::Vertex topRight;
    sf::Vertex bottomLeft;
    sf::Vertex bottomRight;

    float pixelX = x * cellSize;
    float pixelY = y * cellSize;

    topLeft     .position = { pixelX,               pixelY };
    topRight    .position = { pixelX + cellSize,    pixelY };
    bottomLeft  .position = { pixelX,               pixelY + cellSize };
    bottomRight .position = { pixelX + cellSize,    pixelY + cellSize };

    topLeft     .color = myConfig->fgColor;
    topRight    .color = myConfig->fgColor;
    bottomLeft  .color = myConfig->fgColor;
    bottomRight .color = myConfig->fgColor;

    cellVertexPoints.push_back(topLeft);
    cellVertexPoints.push_back(bottomLeft);
    cellVertexPoints.push_back(bottomRight);
    cellVertexPoints.push_back(topRight);
}

/* Public methods */
CellularAutomaton::CellularAutomaton(const Config& config, const Application& app) : myConfig(&config), myApp(&app) {
    cellVertexPoints.reserve(config.simSize.x * config.simSize.y * 4);
    for (unsigned y = 0; y < config.simSize.y; ++y) {
        for (unsigned x = 0; x < config.simSize.x; ++x) {
            addQuad(x, y);
        }
    }
}

void CellularAutomaton::render(sf::RenderWindow& window) {
    onRenderCells(window);
    window.draw(cellVertexPoints.data(), cellVertexPoints.size(), sf::Quads);
    onRenderGUI(window);
}

#endif // CELLULAR_AUTOMATON_H