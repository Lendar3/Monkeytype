#include "game/GameModel.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>

#include "game/GameConfig.hpp"

namespace game {

GameModel::GameModel()
    : state_(GameState::MENU),
      score_(0),
      lives_(3),
      spawnInterval_(kDefaultSpawnInterval),
      input_(),
      targetIndex_(-1),
      fontIndex_(kDefaultFontIndex),
      characterSize_(kDefaultCharacterSize)
{
    loadWords(kWordsFile);
}

void GameModel::loadWords(const std::string& path) {
    wordsBank_.clear();
    std::ifstream wordsBankFile(path);
    std::string s;
    while (wordsBankFile >> s) {
        wordsBank_.push_back(s);
    }
}

void GameModel::startGame() {
    state_ = GameState::PLAYING;
    score_ = 0;
    input_.clear();
    targetIndex_ = -1;
    words_.clear();
    spawnClock_.restart();
    lives_ = 10;
}

void GameModel::setState(GameState state) {
    state_ = state;
}

GameState GameModel::getState() const {
    return state_;
}

void GameModel::update(float dt, const sf::Font& font, unsigned characterSize) {
    if (state_ != GameState::PLAYING) {
        return;
    }

    if (spawnClock_.getElapsedTime().asSeconds() >= spawnInterval_) {
        spawnClock_.restart();
        spawnWord(font, characterSize);
    }

    for (Word& w : words_) {
        w.update(dt);
    }

    for (auto it = words_.begin(); it != words_.end();) {
        if (it->isOffScreen()) {
            it = words_.erase(it);
            input_.clear();
            targetIndex_ = -1;
            lives_--;

            if (lives_ <= 0) {
                state_ = GameState::GAME_OVER;
                words_.clear();
                input_.clear();
                targetIndex_ = -1;
                break;
            }
        } else {
            ++it;
        }
    }

    if (state_ == GameState::GAME_OVER) {
        words_.clear();
        input_.clear();
        targetIndex_ = -1;
    }
}

void GameModel::handleTextInput(char c) {
    if (state_ != GameState::PLAYING) {
        return;
    }

    unsigned char uc = static_cast<unsigned char>(c);
    if (!std::isalpha(uc)) {
        return;
    }

    c = static_cast<char>(std::tolower(uc));

    if (targetIndex_ == -1) {
        for (std::size_t i = 0; i < words_.size(); ++i) {
            if (!words_[i].fullWord.empty() && words_[i].fullWord[0] == c) {
                targetIndex_ = static_cast<int>(i);
                input_.assign(1, c);
                words_[i].setTypedPart(input_);
                break;
            }
        }
        return;
    }

    input_.push_back(c);
    words_[targetIndex_].setTypedPart(input_);

    if (input_ == words_[targetIndex_].fullWord) {
        words_.erase(words_.begin() + targetIndex_);
        targetIndex_ = -1;
        input_.clear();
        score_ += 1;
    }
}

void GameModel::handleBackspace() {
    if (state_ != GameState::PLAYING) {
        return;
    }

    if (input_.empty()) {
        return;
    }

    input_.pop_back();
    if (targetIndex_ != -1) {
        words_[targetIndex_].setTypedPart(input_);
        if (input_.empty()) {
            targetIndex_ = -1;
        }
    }
}

void GameModel::increaseSpawnInterval() {
    if (spawnInterval_ < 5.0f) {
        spawnInterval_ += 0.1f;
    }
}

void GameModel::decreaseSpawnInterval() {
    if (spawnInterval_ > 0.3f) {
        spawnInterval_ -= 0.1f;
    }
}

void GameModel::restartSpawnClock() {
    spawnClock_.restart();
}

void GameModel::cycleFont(int delta, std::size_t fontCount) {
    if (fontCount == 0) {
        return;
    }

    int count = static_cast<int>(fontCount);
    fontIndex_ = (fontIndex_ + delta) % count;
    if (fontIndex_ < 0) {
        fontIndex_ += count;
    }
}

void GameModel::increaseCharacterSize() {
    if (characterSize_ < 50) {
        characterSize_++;
    }
}

void GameModel::decreaseCharacterSize() {
    if (characterSize_ > 10) {
        characterSize_--;
    }
}

int GameModel::getFontIndex() const {
    return fontIndex_;
}

int GameModel::getCharacterSize() const {
    return characterSize_;
}

int GameModel::getScore() const {
    return score_;
}

int GameModel::getLives() const {
    return lives_;
}

float GameModel::getSpawnInterval() const {
    return spawnInterval_;
}

const std::vector<Word>& GameModel::getWords() const {
    return words_;
}

void GameModel::appendScore(const std::string& path) const {
    std::ofstream scoreText;
    scoreText.open(path, std::ios_base::app);
    scoreText << std::to_string(score_) + "\n";
}

void GameModel::spawnWord(const sf::Font& font, unsigned characterSize) {
    if (wordsBank_.empty()) {
        return;
    }

    std::string newWordStr = wordsBank_[static_cast<std::size_t>(std::rand()) % wordsBank_.size()];
    sf::Text tempText(font, newWordStr, characterSize);
    sf::FloatRect textBounds = tempText.getLocalBounds();
    float textHeight = textBounds.size.y;
    float spawnX = static_cast<float>(std::rand() % static_cast<int>(kWindowHeight - textHeight));
    float initialY = -50.0f;
    float wordSpeed = 100.0f + static_cast<float>(score_) * 5.0f;
    words_.emplace_back(newWordStr, font, characterSize, sf::Vector2f(initialY, spawnX), wordSpeed);
}

} // namespace game
