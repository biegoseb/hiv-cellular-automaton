#ifndef VIH_EVOLUTION_H
#define VIH_EVOLUTION_H

#include <vector>
#include "../CellularAutomaton.h"
#include "lymphocyte.h"
#include "../resource-manager/resourceHolder.h"

using std::vector;
using std::pair;

class VIHevolution : public CellularAutomaton {
private:
    enum class Square { On, Off };
    vector<Square> squares;
    vector<Lymphocyte> lymphocytes;
    sf::Text healthyCountText;
    sf::Text identifiedCountText;
    sf::Text latentCountText;
    sf::Text infectedCountText;
    sf::Text deadCountText;
    int healthyCount = 0;
    int identifiedCount = 0;
    int latentCount = 0;
    int infectedCount = 0;
    int deadCount = 0;
    int necessaryIdentifieds;
    float necessaryLatentProb;
    float necessaryDeadProb;
    int timeInfectedToIdentified;
    void updateHealthy(Lymphocyte& thisLymphocyte, int nbIdtCount, int nbInfCount);
    void updateInfected(Lymphocyte& thisLymphocyte);
    void updateIdentified(Lymphocyte& thisLymphocyte);
public:
    VIHevolution(const Config& cfg, const Application& app);
    void update() override;
    void onRenderGUI(sf::RenderWindow& window) override;
};

void initTextVIH(sf::Text& text, float y) {
    text.move(10, y);
    text.setFillColor(sf::Color::White);
    text.setFont(ResourceHolder::get().fonts.get("arial"));
    text.setCharacterSize(18);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2);
}

VIHevolution::VIHevolution(const Config& cfg, const Application& app)
    : CellularAutomaton(cfg, app), lymphocytes(cfg.simSize.x * cfg.simSize.y) {
    necessaryIdentifieds = Random::get().intInRange(1, 8);
    necessaryLatentProb = Random::get().floatInRange(0.35f, 1.0f);
    necessaryDeadProb = Random::get().floatInRange(0.65f, 0.1f);
    timeInfectedToIdentified = Random::get().intInRange(100, 1000);
    /*cellForEach([&](unsigned x, unsigned y) {
        auto index = getCellIndex(x, y);
        auto status = lymphocytes[index].getStatus();
        switch (status) {
        case LymphocyteStatus::healthy:
            ++healthyCount;
            break;
        case LymphocyteStatus::identified:
            ++identifiedCount;
            break;
        case LymphocyteStatus::latent:
            ++latentCount;
            break;
        case LymphocyteStatus::infected:
            ++infectedCount;
            break;
        case LymphocyteStatus::dead:
            ++deadCount;
            break;
        default:
            break;
        }
        sf::Color lymphocyteColor = lymphocytes[index].getColor();
        setCellColor(x, y, lymphocyteColor);
    });*/
    
    std::mt19937 range((unsigned)std::time(nullptr));
    cellForEach([&](unsigned x, unsigned y) {
        unsigned index = getCellIndex(x, y);
        std::uniform_int_distribution<int> dist(0, 1);
        auto& lymphocyte = lymphocytes[index];
        lymphocyte.setStatus((LymphocyteStatus)dist(range));
        CellularAutomaton::setCellColor(x, y, lymphocyte.getStatus() == LymphocyteStatus::healthy ? sf::Color::Green : myConfig->fgColor);
    });
    initTextVIH(healthyCountText, 40);
    initTextVIH(identifiedCountText, 60);
    initTextVIH(latentCountText, 80);
    initTextVIH(infectedCountText, 100);
    initTextVIH(deadCountText, 120);
}

void VIHevolution::update() {
    /*cellForEach([&](unsigned x, unsigned y){
        auto index = getCellIndex(x, y);
        auto& thisLymphocyte = lymphocytes[index];
        auto thisStatus = thisLymphocyte.getStatus();

        if (thisStatus == LymphocyteStatus::dead) return;

        int xChange = Random::get().intInRange(-1, 1);
        int yChange = Random::get().intInRange(-1, 1);
        int xAdj = x + xChange;
        int yAdj = y + yChange;
        if (xAdj < 0 || xAdj >= (int)myConfig->simSize.x) return;
        if (yAdj < 0 || yAdj >= (int)myConfig->simSize.y) return;

        auto adjIndex = getCellIndex(xAdj, yAdj);
        auto& otherLymphocyte = lymphocytes[adjIndex];

    });*/
    vector<pair<sf::Vector2i, LymphocyteStatus>> updates;
    cellForEach([&](unsigned x, unsigned y){
        sf::Vector2i loc(x, y);
        unsigned nbIdtCount = 0;
        unsigned nbInfCount = 0;
        /* Check neighbours */
        for (int nX = -1; nX <= 1; ++nX) {
            for (int nY = -1; nY <= 1; ++nY) {
                int newX = nX + x;
                int newY = nY + y;
                if (newX == -1 || newX == (int)myConfig->simSize.x ||
                    newY == -1 || newY == (int)myConfig->simSize.y || // Out of bounds
                    (nX == 0 && nY == 0)) // Lymphocyte itself
                {
                    continue;
                }

                auto lymphocyte = lymphocytes[getCellIndex(newX, newY)];
                auto lymphStatus = lymphocyte.getStatus();
                if (lymphStatus == LymphocyteStatus::identified) {
                    ++nbIdtCount;
                } else if (lymphStatus == LymphocyteStatus::infected) {
                    ++nbInfCount;
                }
            }
        }

        int index = getCellIndex(x, y);
        auto lymphocyte = lymphocytes[index];
        auto lymphStatus = lymphocyte.getStatus();
        switch (lymphStatus) {
        case LymphocyteStatus::healthy:
            updateHealthy(lymphocyte, nbIdtCount, nbInfCount);
            updates.emplace_back(loc, lymphocyte.getStatus());
            break;
        case LymphocyteStatus::identified:
            updateIdentified(lymphocyte);
            updates.emplace_back(loc, lymphocyte.getStatus());
            break;
        case LymphocyteStatus::infected:
            updateInfected(lymphocyte);
            updates.emplace_back(loc, lymphocyte.getStatus());
            break;
        default:
            break;
        }
        sf::Color lymphocyteColor = lymphocytes[index].getColor();
        setCellColor(x, y, lymphocyteColor);
    });

    for (auto& u : updates) {
        lymphocytes[getCellIndex(u.first.x, u.first.y)].setStatus(u.second);
        CellularAutomaton::setCellColor(u.first.x, u.first.y, u.second == LymphocyteStatus::healthy ? myConfig->bgColor : myConfig->fgColor);
    }
}

void VIHevolution::onRenderGUI(sf::RenderWindow& window) {
    healthyCountText.setString("Healthy Lymphocytes: " + std::to_string(healthyCount));
    identifiedCountText.setString("Identified Lymphocytes: " + std::to_string(identifiedCount));
    latentCountText.setString("Latent Lymphocytes: " + std::to_string(latentCount));
    infectedCountText.setString("Infected Lymphocytes: " + std::to_string(infectedCount));
    deadCountText.setString("Dead Lymphocytes: " + std::to_string(deadCount));
    window.draw(healthyCountText);
    window.draw(identifiedCountText);
    window.draw(latentCountText);
    window.draw(infectedCountText);
    window.draw(deadCountText);
}

void VIHevolution::updateHealthy(Lymphocyte& thisLymphocyte, int nbIdtCount, int nbInfCount) {
    if (nbInfCount > 1 || nbIdtCount == necessaryIdentifieds) {
        if (thisLymphocyte.getLatentProb() >= necessaryLatentProb) {
            thisLymphocyte.setStatus(LymphocyteStatus::latent);
            ++latentCount;
        } else {
            thisLymphocyte.setStatus(LymphocyteStatus::infected);
            ++infectedCount;
        }
        --healthyCount;
    } else if (thisLymphocyte.getDeadProb() >= necessaryDeadProb) {
        thisLymphocyte.setStatus(LymphocyteStatus::dead);
        ++deadCount;
    } else {
        thisLymphocyte.setStatus(LymphocyteStatus::healthy);
    }
}

void VIHevolution::updateInfected(Lymphocyte& thisLymphocyte) {
    if (thisLymphocyte.getStatusNumber() % timeInfectedToIdentified == 0) {
        thisLymphocyte.setStatus(LymphocyteStatus::identified);
        ++identifiedCount;
    } else {
        thisLymphocyte.setStatus(LymphocyteStatus::infected);
    }
}

void VIHevolution::updateIdentified(Lymphocyte& thisLymphocyte) {
    if (thisLymphocyte.getStatus() == LymphocyteStatus::identified) {
        thisLymphocyte.setStatus(LymphocyteStatus::dead);
        ++deadCount;
    } else {
        thisLymphocyte.setStatus(LymphocyteStatus::identified);
    }
}

#endif // VIH_EVOLUTION_H