#include "game/Word.hpp"

namespace game {

Word::Word(const std::string& word, const sf::Font& font, unsigned size, sf::Vector2f position, float spd)
    : fullWord(word), typedPart(""), speed(spd),
      typedText(font, "", size), remainingText(font, word, size)
{
    typedText.setFillColor(sf::Color::Green);
    remainingText.setFillColor(sf::Color::White);
    typedText.setPosition(position);
    remainingText.setPosition(position);
}

void Word::update(float dt) {
    typedText.move({speed * dt, 0.0f});
    remainingText.move({speed * dt, 0.0f});
}

void Word::setTypedPart(const std::string& input) {
    typedPart = input;
    typedText.setString(typedPart);

    bool isCorrect = fullWord.starts_with(input);
    typedText.setFillColor(isCorrect ? sf::Color::Green : sf::Color::Red);

    std::string rest = input.size() <= fullWord.size() ? fullWord.substr(input.size()) : "";
    remainingText.setString(rest);

    float typedWidth = typedText.getLocalBounds().size.x;
    float typedOffset = typedText.getLocalBounds().position.x;
    float remainingOffset = remainingText.getLocalBounds().position.x;
    sf::Vector2f basePos = typedText.getPosition();
    remainingText.setPosition(sf::Vector2f(basePos.x + typedWidth - typedOffset - remainingOffset, basePos.y));
}

void Word::reset() {
    typedPart = "";
    typedText.setString("");
    typedText.setFillColor(sf::Color::Green);
    remainingText.setString(fullWord);
    remainingText.setFillColor(sf::Color::White);
}

bool Word::isOffScreen() const {
    return typedText.getPosition().x + sf::FloatRect(typedText.getLocalBounds()).position.x +
           sf::FloatRect(remainingText.getLocalBounds()).position.x > kWindowWidth;
}

} // namespace game
