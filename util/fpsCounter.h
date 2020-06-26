#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../resource-manager/resourceHolder.h"

class FPSCounter {
    private:
        sf::Text myText;
        sf::Font myFont;
        sf::Clock myDelayTimer;
        sf::Clock myFpsTimer;
        float myFps = 0;
        int myFrameCount = 0;
    public:
        FPSCounter();
        void update();
        void draw(sf::RenderTarget& renderer);
};

FPSCounter::FPSCounter() {
    myText.move             (10, 20);
    myText.setFillColor     (sf::Color::White);
    myText.setFont          (ResourceHolder::get().fonts.get("arial"));
    myText.setCharacterSize (18);
    myText.setOutlineColor  (sf::Color::Black);
    myText.setOutlineThickness(2);
}

void FPSCounter::update() {
    ++myFrameCount;
    if (myDelayTimer.getElapsedTime().asSeconds() > 0.2) {
        myFps = myFrameCount / myFpsTimer.restart().asSeconds();
        myFrameCount = 0;
        myDelayTimer.restart();
    }
}

void FPSCounter::draw(sf::RenderTarget& renderer) {
    myText.setString("FPS: " + std::to_string((int)myFps));
    renderer.draw(myText);
}

#endif // FPS_COUNTER_H