#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "game/GameState.hpp"
#include "game/Word.hpp"

namespace game {

class GameModel {
public:
    GameModel();

    void loadWords(const std::string& path);

    void startGame();
    void setState(GameState state);
    GameState getState() const;

    void update(float dt, const sf::Font& font, unsigned characterSize);

    void handleTextInput(char c);
    void handleBackspace();

    void increaseSpawnInterval();
    void decreaseSpawnInterval();

    void restartSpawnClock();

    void cycleFont(int delta, std::size_t fontCount);
    void increaseCharacterSize();
    void decreaseCharacterSize();

    int getFontIndex() const;
    int getCharacterSize() const;

    int getScore() const;
    int getLives() const;
    float getSpawnInterval() const;
    const std::vector<Word>& getWords() const;

    void appendScore(const std::string& path) const;

private:
    void spawnWord(const sf::Font& font, unsigned characterSize);

    GameState state_;
    std::vector<Word> words_;
    std::vector<std::string> wordsBank_;
    sf::Clock spawnClock_;
    int score_;
    int lives_;
    float spawnInterval_;
    std::string input_;
    int targetIndex_;
    int fontIndex_;
    int characterSize_;
};

} // namespace game
