#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <SFML/Graphics.hpp>
#include "resourceManager.h"

class ResourceHolder {
    private:
        ResourceHolder();
    public:
        static ResourceHolder& get();
        ResourceManager<sf::Font> fonts;
        ResourceManager<sf::Texture> textures;   
};

ResourceHolder::ResourceHolder() : fonts("Fonts", "ttf"), textures("Textures", "png") {}

ResourceHolder& ResourceHolder::get() {
    static ResourceHolder holder;
    return holder;
}

#endif // RESOURCE_HOLDER_H