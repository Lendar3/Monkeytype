#include "game/GameView.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <ranges>

namespace game {
namespace {

std::vector<sf::Font> loadFonts() {
    std::vector<sf::Font> fonts;
    fonts.emplace_back(kFontArial);
    fonts.emplace_back(kFontSFFedora);
    fonts.emplace_back(kFontPlayfair);
    return fonts;
}

std::vector<sf::Texture> loadBackgrounds() {
    std::vector<sf::Texture> textures(3);
    textures[0].loadFromFile(kBackground1);
    textures[1].loadFromFile(kBackground2);
    textures[2].loadFromFile(kBackground3);
    return textures;
}

} // namespace

GameView::GameView(int initialFontIndex, int initialCharacterSize)
    : fonts_(loadFonts()),
      backgrounds_(loadBackgrounds()),
      backgroundIndex_(1),
      backgroundSprite_(backgrounds_[backgroundIndex_]),
      title_(fonts_[initialFontIndex], "MonkeyTyper", 48),
      rules_(fonts_[initialFontIndex],
             "RULES: \n\n1.If you want to leave the game or take a rest, use pause by ESC \n \n2.If you are leaving the game by PAUSE your score won't be saved \n \n3.In settings you can use chose font-size and font for the game \n \n4.During the game speed of words automatically increase accorrding to your score \n\n5.if you want more words during the game or less use by arrows up/down to change  \n\n6.You have 3 lives in one game\n\n7.By combination Lalt+4 and Lalt+5, you can change background instantly  \n\n8. GOOD LUCK :) ",
             17),
      startButton_(fonts_[initialFontIndex], "Start", 28, {150.0f, 50.0f},
                   {kWindowWidth * 0.05f, kWindowHeight * 0.3f}),
      settingsButton_(fonts_[initialFontIndex], "Settings", 28, {150.0f, 50.0f},
                      {kWindowWidth * 0.05f, kWindowHeight * 0.45f}),
      leadershipButton_(fonts_[initialFontIndex], "Leadership", 25, {150.0f, 50.0f},
                        {kWindowWidth * 0.05f, kWindowHeight * 0.6f}),
      exitButton_(fonts_[initialFontIndex], "Exit", 28, {150.0f, 50.0f},
                  {kWindowWidth * 0.05f, kWindowHeight * 0.75f}),
      returnButton_(fonts_[initialFontIndex], "Return", 28, {150.0f, 50.0f},
                    {kWindowWidth * 0.75f, kWindowHeight * 0.8f}),
      resumeButton_(fonts_[initialFontIndex], "Resume", 28, {150.0f, 50.0f},
                    {kWindowWidth * 0.45f, kWindowHeight * 0.2f}),
      menuButton_(fonts_[initialFontIndex], "Menu", 28, {150.0f, 50.0f},
                  {kWindowWidth * 0.45f, kWindowHeight * 0.6f}),
      exampleTextChange_(fonts_[initialFontIndex], "Example  of text style for the game",
                         initialCharacterSize, {kWindowWidth, 100.0f}, {0.0f, kWindowHeight * 0.2f}),
      left_(38.0f, {kWindowWidth * 0.3f, kWindowHeight * 0.6f}, 270.f),
      right_(38.0f, {kWindowWidth * 0.6f, kWindowHeight * 0.6f}, 90.f),
      up_(38.0f, {kWindowWidth * 0.45f, kWindowHeight * 0.45f}, 0.f),
      down_(38.0f, {kWindowWidth * 0.45f, kWindowHeight * 0.75f}, 180.f),
      gameOverText_(fonts_[initialFontIndex], "Game Over!", 42),
      scoreMsgText_(fonts_[initialFontIndex], "", 30),
      continueText_(fonts_[initialFontIndex], "Press Enter to return to menu", 20),
      scoreText_(fonts_[initialFontIndex], "Score: 0", 24),
      livesText_(fonts_[initialFontIndex], "Lives: 3", 24),
      spawnText_(fonts_[initialFontIndex], "Spawn: " + std::to_string(kDefaultSpawnInterval), 24),
      leaderboardText_(fonts_[initialFontIndex], "", 24)
{
    sf::FloatRect titleBounds = title_.getLocalBounds();
    title_.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.0f,
                      titleBounds.position.y + titleBounds.size.y / 2.0f});
    title_.setPosition({kWindowWidth / 2.0f, kWindowHeight * 0.2f});
    title_.setFillColor(sf::Color::Yellow);

    sf::FloatRect rulesBounds = rules_.getLocalBounds();
    rules_.setOrigin({rulesBounds.position.x + rulesBounds.size.x / 2.0f,
                      rulesBounds.position.y + rulesBounds.size.y / 2.0f});
    rules_.setPosition({kWindowWidth * 0.75f, kWindowHeight * 0.6f});
    rules_.setFillColor(sf::Color::White);

    menuButtons_ = {&startButton_, &settingsButton_, &leadershipButton_, &exitButton_};

    exampleTextChange_.shape.setFillColor(sf::Color::Magenta);

    sf::FloatRect goBounds = gameOverText_.getLocalBounds();
    gameOverText_.setOrigin({goBounds.position.x + goBounds.size.x / 2.0f,
                             goBounds.position.y + goBounds.size.y / 2.0f});
    gameOverText_.setPosition({kWindowWidth / 2.0f, kWindowHeight * 0.2f});
    gameOverText_.setFillColor(sf::Color::Red);

    scoreMsgText_.setFillColor(sf::Color::White);

    sf::FloatRect ctBounds = continueText_.getLocalBounds();
    continueText_.setOrigin({ctBounds.position.x + ctBounds.size.x / 2.0f, 0});
    continueText_.setPosition({kWindowWidth / 2.0f, kWindowHeight * 0.5f});
    continueText_.setFillColor(sf::Color::Cyan);

    scoreText_.setPosition({10.0f, 10.0f});
    scoreText_.setFillColor(sf::Color::White);

    livesText_.setFillColor(sf::Color::Magenta);
    livesText_.setPosition({10.0f, 40.0f});

    spawnText_.setPosition({10.0f, 70.0f});
    spawnText_.setFillColor(sf::Color::White);

    leaderboardText_.setFillColor(sf::Color::White);
}

const sf::Font& GameView::fontAt(int index) const {
    return fonts_.at(static_cast<std::size_t>(index));
}

std::size_t GameView::fontCount() const {
    return fonts_.size();
}

void GameView::updateBackground(int delta) {
    if (backgrounds_.empty()) {
        return;
    }

    if (delta > 0) {
        if (backgroundIndex_ < static_cast<int>(backgrounds_.size()) - 1) {
            backgroundIndex_++;
        }
    } else if (delta < 0) {
        if (backgroundIndex_ > 0) {
            backgroundIndex_--;
        }
    }

    backgroundSprite_.setTexture(backgrounds_[backgroundIndex_]);
}

MenuAction GameView::hitTestMenu(sf::Vector2f point) const {
    if (startButton_.contains(point)) {
        return MenuAction::Start;
    }
    if (settingsButton_.contains(point)) {
        return MenuAction::Settings;
    }
    if (leadershipButton_.contains(point)) {
        return MenuAction::Leaderboard;
    }
    if (exitButton_.contains(point)) {
        return MenuAction::Exit;
    }
    return MenuAction::None;
}

SettingsAction GameView::hitTestSettings(sf::Vector2f point) const {
    if (left_.contains(point)) {
        return SettingsAction::FontPrev;
    }
    if (right_.contains(point)) {
        return SettingsAction::FontNext;
    }
    if (up_.contains(point)) {
        return SettingsAction::SizeUp;
    }
    if (down_.contains(point)) {
        return SettingsAction::SizeDown;
    }
    return SettingsAction::None;
}

bool GameView::hitTestReturn(sf::Vector2f point) const {
    return returnButton_.contains(point);
}

PauseAction GameView::hitTestPause(sf::Vector2f point) const {
    if (resumeButton_.contains(point)) {
        return PauseAction::Resume;
    }
    if (menuButton_.contains(point)) {
        return PauseAction::Menu;
    }
    return PauseAction::None;
}

void GameView::updateHover(sf::Vector2f mousePos) {
    for (ui::Button* btn : menuButtons_) {
        btn->setHover(btn->contains(mousePos));
    }
    returnButton_.setHover(returnButton_.contains(mousePos));
}

void GameView::applySettings(int fontIndex, int characterSize) {
    exampleTextChange_.label.setCharacterSize(characterSize);
    exampleTextChange_.label.setFont(fonts_[fontIndex]);

    returnButton_.label.setFont(fonts_[fontIndex]);
    startButton_.label.setFont(fonts_[fontIndex]);
    settingsButton_.label.setFont(fonts_[fontIndex]);
    leadershipButton_.label.setFont(fonts_[fontIndex]);
    exitButton_.label.setFont(fonts_[fontIndex]);
    title_.setFont(fonts_[fontIndex]);
}

void GameView::draw(sf::RenderWindow& window, const GameModel& model) {
    window.draw(backgroundSprite_);

    GameState state = model.getState();

    if (state == GameState::MENU) {
        window.draw(title_);
        for (ui::Button* btn : menuButtons_) {
            window.draw(btn->shape);
            window.draw(btn->label);
        }
        window.draw(rules_);
        return;
    }

    if (state == GameState::LEADERBOARD) {
        leaderboardText_.setString(buildLeaderboardText());
        sf::FloatRect lbBounds = leaderboardText_.getLocalBounds();
        leaderboardText_.setOrigin({lbBounds.position.x + lbBounds.size.x / 2.0f, 0});
        leaderboardText_.setPosition({kWindowWidth / 2.0f, kWindowHeight * 0.1f});

        window.draw(leaderboardText_);
        window.draw(returnButton_.shape);
        window.draw(returnButton_.label);
        return;
    }

    if (state == GameState::SETTINGS) {
        window.draw(exampleTextChange_.shape);
        window.draw(exampleTextChange_.label);
        window.draw(returnButton_.shape);
        window.draw(returnButton_.label);
        window.draw(left_.shape);
        window.draw(right_.shape);
        window.draw(up_.shape);
        window.draw(down_.shape);
        return;
    }

    if (state == GameState::PLAYING || state == GameState::PAUSE) {
        for (const Word& w : model.getWords()) {
            window.draw(w.remainingText);
            window.draw(w.typedText);
        }

        scoreText_.setString("Score: " + std::to_string(model.getScore()));
        livesText_.setString("Lives: " + std::to_string(model.getLives()));
        spawnText_.setString("Spawn: " + std::to_string(model.getSpawnInterval()));

        window.draw(scoreText_);
        window.draw(livesText_);
        window.draw(spawnText_);

        if (state == GameState::PAUSE) {
            window.draw(menuButton_.shape);
            window.draw(menuButton_.label);
            window.draw(resumeButton_.shape);
            window.draw(resumeButton_.label);
        }
        return;
    }

    if (state == GameState::GAME_OVER) {
        scoreMsgText_.setString("Your Score: " + std::to_string(model.getScore()));
        sf::FloatRect smBounds = scoreMsgText_.getLocalBounds();
        scoreMsgText_.setOrigin({smBounds.position.x + smBounds.size.x / 2.0f,
                                 smBounds.position.y + smBounds.size.y / 2.0f});
        scoreMsgText_.setPosition({kWindowWidth / 2.0f, kWindowHeight * 0.4f});

        window.draw(gameOverText_);
        window.draw(scoreMsgText_);
        window.draw(continueText_);
    }
}

std::string GameView::buildLeaderboardText() const {
    std::vector<int> highScores;
    std::ifstream leaderboardFile(kLeaderboardFile);
    int s;
    while (leaderboardFile >> s) {
        highScores.push_back(s);
    }

    std::ranges::sort(highScores.begin(), highScores.end(), std::greater<int>());
    if (highScores.size() > 5) {
        highScores.resize(5);
    }

    std::string lbString = "High Scores:\n";
    if (highScores.empty()) {
        lbString += "No scores yet!";
    } else {
        for (std::size_t i = 0; i < highScores.size(); ++i) {
            lbString += std::to_string(i + 1) + ". " + std::to_string(highScores[i]) + "\n";
        }
    }

    return lbString;
}

} // namespace game
