#include "game/ui/Button.hpp"

namespace game::ui {

Button::Button(const sf::Font& font, const std::string& text, unsigned int charSize,
               sf::Vector2f size, sf::Vector2f position)
    : label(font, text, charSize)
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(100, 100, 100));

    sf::FloatRect labelBounds = label.getLocalBounds();
    label.setOrigin({labelBounds.position.x + labelBounds.size.x / 2.0f,
                     labelBounds.position.y + labelBounds.size.y / 2.0f});
    label.setPosition({position.x + size.x / 2.0f, position.y + size.y / 2.0f});
    label.setFillColor(sf::Color::Red);

    shape.setOutlineThickness(4.0f);
    shape.setOutlineColor(sf::Color::Blue);
}

bool Button::contains(sf::Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

void Button::setHover(bool hovering) {
    if (hovering) {
        shape.setFillColor(sf::Color(150, 150, 150));
    } else {
        shape.setFillColor(sf::Color(100, 100, 100));
    }
}

} // namespace game::ui
