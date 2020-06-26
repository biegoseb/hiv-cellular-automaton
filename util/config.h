#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <iostream>

struct Config {
    Config() = default;
    Config(const sf::Vector2u& windowSize, unsigned cellSize);
    void init();
    sf::Vector2u windowSize = {1280, 720};
    sf::Vector2u simSize;
    sf::Color bgColor = {150, 150, 150};
    sf::Color fgColor = {25, 25, 25};
    unsigned cellSize = 4;
    unsigned fps = 30;  
};

Config::Config(const sf::Vector2u& windowSize, unsigned cellSize) : windowSize(windowSize), cellSize(cellSize) {
    init();
}

void Config::init() {
    windowSize.x -= windowSize.x % cellSize;
    windowSize.y -= windowSize.y % cellSize;
    simSize.x = windowSize.x / cellSize;
    simSize.y = windowSize.y / cellSize;
}

#endif // CONFIG_H