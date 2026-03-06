#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "game/GameConfig.hpp"

namespace game {

class Word {
public:
    sf::Text typedText;
    sf::Text remainingText;
    std::string fullWord;
    std::string typedPart;
    float speed;

    Word(const std::string& word, const sf::Font& font, unsigned size, sf::Vector2f position, float spd);

    void update(float dt);
    void setTypedPart(const std::string& input);
    void reset();
    bool isOffScreen() const;
};

} // namespace game
