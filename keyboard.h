#include <array>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

class Keyboard {
    private:
        std::array<bool, sf::Keyboard::KeyCount> m_keys;
    public:
        Keyboard();
        void update(sf::Event e);
        bool isKeyDown(sf::Keyboard::Key key) const;    
};

Keyboard::Keyboard() {
    std::fill(m_keys.begin(), m_keys.end(), false);
}

void Keyboard::update(sf::Event e) {
    switch (e.type) {
        case sf::Event::KeyPressed:
            m_keys[e.key.code] = true;
            break;
        case sf::Event::KeyReleased:
            m_keys[e.key.code] = false;
            break;
        default:
            break;
    }
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key) const {
    return m_keys.at(key);
}