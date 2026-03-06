#pragma once

#include <SFML/Graphics.hpp>

namespace game::ui {

class Triangle {
public:
    sf::CircleShape shape;

    Triangle(float size, sf::Vector2f position, float rotation);

    bool contains(sf::Vector2f point) const;
    void draw(sf::RenderWindow& window) const;
};

} // namespace game::ui
