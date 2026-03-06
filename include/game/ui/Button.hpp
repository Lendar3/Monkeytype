#pragma once

#include <SFML/Graphics.hpp>
#include <string>

namespace game::ui {

class Button {
public:
    sf::RectangleShape shape;
    sf::Text label;

    Button(const sf::Font& font, const std::string& text, unsigned int charSize,
           sf::Vector2f size, sf::Vector2f position);

    bool contains(sf::Vector2f point) const;
    void setHover(bool hovering);
};

} // namespace game::ui
