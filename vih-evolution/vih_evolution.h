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
    void updateHealthy(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte, int nbIdtCount, int nbInfCount);
    void updateInfected(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte);
    void updateIdentified(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte);
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
    necessaryIdentifieds = 2;
    necessaryLatentProb = Random::get().floatInRange(0, 1);
    necessaryDeadProb = Random::get().floatInRange(0, 1);
    timeInfectedToIdentified = Random::get().intInRange(0, 10);
    
    cellForEach([&](unsigned x, unsigned y) {
        unsigned index = getCellIndex(x, y);
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
        CellularAutomaton::setCellColor(x, y, lymphocytes[index].getColor());
    });
    initTextVIH(healthyCountText, 40);
    //initTextVIH(identifiedCountText, 60);
    //initTextVIH(latentCountText, 80);
    initTextVIH(infectedCountText, 60);
    initTextVIH(deadCountText, 80);
}

void VIHevolution::update() {
    cellForEach([&](unsigned x, unsigned y){
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
                if (lymphocyte.getStatus() == LymphocyteStatus::identified) {
                    ++nbIdtCount;
                } else if (lymphocyte.getStatus() == LymphocyteStatus::infected) {
                    ++nbInfCount;
                }
            }
        }

        int index = getCellIndex(x, y);
        Lymphocyte& lymphocyte = lymphocytes[index];
        if (lymphocyte.getStatus() == LymphocyteStatus::dead) return;

        int xChange = Random::get().intInRange(-1, 1);
        int yChange = Random::get().intInRange(-1, 1);
        int xAdj = x + xChange;
        int yAdj = y + yChange;

        if (xAdj < 0 || xAdj >= (int)myConfig->simSize.x) return;
        if (yAdj < 0 || yAdj >= (int)myConfig->simSize.y) return;

        auto adjIndex = getCellIndex(xAdj, yAdj);
        auto& otherLymphocyte = lymphocytes[adjIndex];

        switch (lymphocyte.getStatus()) {
        case LymphocyteStatus::healthy:
            updateHealthy(lymphocyte, otherLymphocyte, nbIdtCount, nbInfCount);
            break;
        case LymphocyteStatus::identified:
            updateIdentified(lymphocyte, otherLymphocyte);
            break;
        case LymphocyteStatus::infected:
            updateInfected(lymphocyte, otherLymphocyte);
            break;
        default:
            break;
        }
        sf::Color lymphocyteColor = lymphocytes[index].getColor();
        setCellColor(x, y, lymphocyteColor);
    });
}

void VIHevolution::onRenderGUI(sf::RenderWindow& window) {
    healthyCountText.setString("Healthy Lymphocytes: " + std::to_string(healthyCount));
    //identifiedCountText.setString("Latent Lymphocytes: " + std::to_string(identifiedCount));
    //latentCountText.setString("Identified Lymphocytes: " + std::to_string(latentCount));
    infectedCountText.setString("Infected Lymphocytes: " + std::to_string(infectedCount));
    deadCountText.setString("Dead Lymphocytes: " + std::to_string(deadCount));
    window.draw(healthyCountText);
    //window.draw(identifiedCountText);
    //window.draw(latentCountText);
    window.draw(infectedCountText);
    window.draw(deadCountText);
}

void VIHevolution::updateHealthy(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte, int nbIdtCount, int nbInfCount) {
    auto otherStatus = otherLymphocyte.getStatus();
    switch (otherStatus) {
    case LymphocyteStatus::infected:
        ++infectedCount;
        --healthyCount;
        thisLymphocyte.setStatus(LymphocyteStatus::infected);
        break;

    case LymphocyteStatus::latent:
        if (healthyCount > 0 && (nbInfCount > 1 || nbIdtCount == necessaryIdentifieds)) {
            if (thisLymphocyte.getLatentProb() >= necessaryLatentProb) {
                thisLymphocyte.setStatus(LymphocyteStatus::latent);
                ++latentCount;
            } else {
                thisLymphocyte.setStatus(LymphocyteStatus::infected);
                ++infectedCount;
            }
            --healthyCount;
        }
        break;

    case LymphocyteStatus::dead:
        if (thisLymphocyte.getDeadProb() >= necessaryDeadProb) {
            thisLymphocyte.move(otherLymphocyte);
            ++deadCount;
        } else {
            thisLymphocyte.setStatus(LymphocyteStatus::dead);
            otherLymphocyte.setStatus(LymphocyteStatus::healthy);
        }  
        break;
    }
}

void VIHevolution::updateInfected(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte) {
    auto otherStatus = otherLymphocyte.getStatus();
    switch (otherStatus) {
    case LymphocyteStatus::healthy:
        ++infectedCount;
        --healthyCount;
        otherLymphocyte.setStatus(LymphocyteStatus::infected);
        break;

    case LymphocyteStatus::identified:
        if (thisLymphocyte.getStatusNumber() % timeInfectedToIdentified == 0 && identifiedCount > 0) {
            --identifiedCount;
            thisLymphocyte.setStatus(LymphocyteStatus::dead);
            ++deadCount;
        }
        break;
    case LymphocyteStatus::latent:
        if (latentCount > 0) {
            ++infectedCount;
            --latentCount;
            otherLymphocyte.setStatus(LymphocyteStatus::infected);
        }
        break;

    case LymphocyteStatus::dead:
        thisLymphocyte.move(otherLymphocyte);
        break;
    }
}

void VIHevolution::updateIdentified(Lymphocyte& thisLymphocyte, Lymphocyte& otherLymphocyte) {
    auto otherStatus = otherLymphocyte.getStatus();
    if (otherStatus == LymphocyteStatus::identified) {
        --identifiedCount;
        otherLymphocyte.setStatus(LymphocyteStatus::dead);
    } else if (otherStatus == LymphocyteStatus::dead) {
        thisLymphocyte.move(otherLymphocyte);
    }
}

#endif // VIH_EVOLUTION_H