#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <memory>
#include <thread>
#include "CellularAutomaton.h"
#include "util/fpsCounter.h"
#include "keyboard.h"
#include "util/random.h"
#include "util/config.h"
#include "native/unix.h"

struct Config;
class Application {
    private:
        void pollEvents();
        void input(float dt);
        void render();
        void resetView();
        std::unique_ptr<CellularAutomaton> myAutomaton;
        sf::RenderWindow myWindow;
        FPSCounter myFpsCounter;
        Keyboard myKeyboard;
        sf::Text myGuiText;
        sf::View myView;
        const Config* myConfig = nullptr;
        float myZoom = 1;
    public:
        Application(const Config& config);
        template<typename T> void init();
        void run();
        const sf::RenderWindow& getWindow() const;
};

/* Private methods */
void Application::pollEvents() {
    sf::Event e;
    while (myWindow.pollEvent(e)) {
        myAutomaton->input(e);
        myKeyboard.update(e);
        switch (e.type) {
            case sf::Event::Closed:
                myWindow.close();
                break;
        }
    }
}

void Application::input(float dt) {
    float speed = 250;
    sf::Vector2f change;
    if (myKeyboard.isKeyDown(sf::Keyboard::W)) {
        change.y -= speed;
    }
    else if (myKeyboard.isKeyDown(sf::Keyboard::S)) {
        change.y += speed;
    }
    if (myKeyboard.isKeyDown(sf::Keyboard::A)) {
        change.x -= speed;
    }
    else if (myKeyboard.isKeyDown(sf::Keyboard::D)) {
        change.x += speed;
    }

    if (myKeyboard.isKeyDown(sf::Keyboard::P)) {
        //std::thread(&Application::makeImage, this).detach();
    }
    if (myKeyboard.isKeyDown(sf::Keyboard::Up)) {
        myView.zoom(0.95f);
    }
    if (myKeyboard.isKeyDown(sf::Keyboard::Down)) {
        myView.zoom(1.05f);
    }
    if (myKeyboard.isKeyDown(sf::Keyboard::R)) {
        resetView();
    }
    myView.move(change * dt);
}

void Application::render() {
    myWindow.setView(myView);
    myWindow.clear(myConfig->bgColor);

    //Pixels
    myWindow.setView(myView);
    myAutomaton->render(myWindow);

    //GUI
    myWindow.setView(myWindow.getDefaultView());
    myWindow.draw(myGuiText);
    myFpsCounter.draw(myWindow);

    myWindow.display();
}

void Application::resetView() {
    myView = sf::View();
    myView.setCenter({ (float)myConfig->windowSize.x / 2,  (float)myConfig->windowSize.y / 2 });
    myView.setSize({ (float)myConfig->windowSize.x,        (float)myConfig->windowSize.y });
}

/* Public methods */
Application::Application(const Config& config)
    : myWindow({ config.windowSize.x, config.windowSize.y }, "Cellular Automaton"), myConfig(&config) {
    resetView();
    myGuiText.setFont(ResourceHolder::get().fonts.get("arial"));
    myGuiText.move(10, 3);
    myGuiText.setCharacterSize(18);
    myGuiText.setOutlineColor(sf::Color::Black);
    myGuiText.setOutlineThickness(2);
    myWindow.setFramerateLimit(config.fps);
}

template<typename T> 
void Application::init() {
    myAutomaton = std::make_unique<T>(*myConfig, *this);
}

void Application::run() {
    sf::Clock deltaClock;
    unsigned year = 0;
    myWindow.setKeyRepeatEnabled(false);
    while (myWindow.isOpen()) {
        myGuiText.setString("Generation: " + std::to_string(year++));
        myFpsCounter.update();
        input (deltaClock.restart().asSeconds());
        myAutomaton->update();
        render();
        pollEvents();
    }
}

const sf::RenderWindow& Application::getWindow() const {
    return myWindow;
}

#endif // APPLICATION_H