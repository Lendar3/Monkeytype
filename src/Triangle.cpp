#include "game/ui/Triangle.hpp"

namespace game::ui {

Triangle::Triangle(float size, sf::Vector2f position, float rotation)
    : shape(size, 3)
{
    sf::FloatRect shapeBounds = shape.getLocalBounds();
    shape.setOrigin({shapeBounds.position.x + shapeBounds.size.x / 2.0f,
                     shapeBounds.position.y + shapeBounds.size.y / 2.0f});
    shape.setPosition(position);
    shape.setRotation(sf::degrees(rotation));
    shape.setFillColor(sf::Color::Green);
}

bool Triangle::contains(sf::Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

void Triangle::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

} // namespace game::ui
