#include "game/GameController.hpp"

#include "game/GameConfig.hpp"

#include <cctype>

namespace {

std::optional<char> keyToLetter(sf::Keyboard::Key code) {
    if (code < sf::Keyboard::Key::A || code > sf::Keyboard::Key::Z) {
        return std::nullopt;
    }
    int offset = static_cast<int>(code) - static_cast<int>(sf::Keyboard::Key::A);
    return static_cast<char>('a' + offset);
}

} // namespace

namespace game {

GameController::GameController()
    : window_(sf::VideoMode({kWindowWidth, kWindowHeight}), "MonkeyTyper",
              sf::Style::Default, sf::State::Windowed,
              sf::ContextSettings{.antiAliasingLevel = 8}),
      model_(),
      view_(kDefaultFontIndex, kDefaultCharacterSize),
      frameClock_()
{
    window_.setFramerateLimit(60);
}

void GameController::run() {
    while (window_.isOpen()) {
        while (auto const event = window_.pollEvent()) {
            processEvent(*event);
        }
        update();
        render();
    }
}

void GameController::processEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        window_.close();
        return;
    }

    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        handleKeyPressed(*keyEvent);
    }

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        handleTextEntered(*textEvent);
    }

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        handleMousePressed(*mouseEvent);
    }

    if (const auto* moveEvent = event.getIf<sf::Event::MouseMoved>()) {
        handleMouseMoved(*moveEvent);
    }
}

void GameController::handleKeyPressed(const sf::Event::KeyPressed& keyEvent) {
    if (keyEvent.code == sf::Keyboard::Key::Num4 &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)) {
        view_.updateBackground(1);
    }
    if (keyEvent.code == sf::Keyboard::Key::Num5 &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)) {
        view_.updateBackground(-1);
    }

    GameState state = model_.getState();

    if (state == GameState::PLAYING) {
        if (keyEvent.code == sf::Keyboard::Key::Escape) {
            model_.setState(GameState::PAUSE);
        }
        if (keyEvent.code == sf::Keyboard::Key::Backspace) {
            model_.handleBackspace();
        }
        if (keyEvent.code == sf::Keyboard::Key::Down) {
            model_.increaseSpawnInterval();
        }
        if (keyEvent.code == sf::Keyboard::Key::Up) {
            model_.decreaseSpawnInterval();
        }

        if (!textEnteredSeen_) {
            if (auto letter = keyToLetter(keyEvent.code)) {
                pendingFallbackChar_ = *letter;
                model_.handleTextInput(*letter);
            }
        }
        return;
    }

    if (state == GameState::GAME_OVER) {
        if (keyEvent.code == sf::Keyboard::Key::Enter ||
            keyEvent.code == sf::Keyboard::Key::Escape) {
            model_.setState(GameState::MENU);
            model_.appendScore(kLeaderboardFile);
        }
    }
}

void GameController::handleTextEntered(const sf::Event::TextEntered& textEvent) {
    if (model_.getState() != GameState::PLAYING) {
        return;
    }

    if (textEvent.unicode < 128) {
        char c = static_cast<char>(textEvent.unicode);
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalpha(uc)) {
            textEnteredSeen_ = true;
        }

        if (pendingFallbackChar_ && *pendingFallbackChar_ == static_cast<char>(std::tolower(uc))) {
            pendingFallbackChar_.reset();
            return;
        }
        pendingFallbackChar_.reset();

        model_.handleTextInput(c);
    }
}

void GameController::handleMousePressed(const sf::Event::MouseButtonPressed& mouseEvent) {
    GameState state = model_.getState();

    if (state == GameState::GAME_OVER) {
        model_.setState(GameState::MENU);
        return;
    }

    if (mouseEvent.button != sf::Mouse::Button::Left) {
        return;
    }

    sf::Vector2f clickPos = static_cast<sf::Vector2f>(mouseEvent.position);

    if (state == GameState::MENU) {
        MenuAction action = view_.hitTestMenu(clickPos);
        if (action == MenuAction::Start) {
            model_.startGame();
            frameClock_.restart();
            return;
        }
        if (action == MenuAction::Leaderboard) {
            model_.setState(GameState::LEADERBOARD);
            return;
        }
        if (action == MenuAction::Settings) {
            model_.setState(GameState::SETTINGS);
            return;
        }
        if (action == MenuAction::Exit) {
            window_.close();
            return;
        }
    }

    if (state == GameState::LEADERBOARD || state == GameState::SETTINGS) {
        if (view_.hitTestReturn(clickPos)) {
            model_.setState(GameState::MENU);
            return;
        }

        if (state == GameState::SETTINGS) {
            SettingsAction action = view_.hitTestSettings(clickPos);
            if (action == SettingsAction::FontPrev) {
                model_.cycleFont(-1, view_.fontCount());
            } else if (action == SettingsAction::FontNext) {
                model_.cycleFont(1, view_.fontCount());
            } else if (action == SettingsAction::SizeUp) {
                model_.increaseCharacterSize();
            } else if (action == SettingsAction::SizeDown) {
                model_.decreaseCharacterSize();
            }

            view_.applySettings(model_.getFontIndex(), model_.getCharacterSize());
        }
        return;
    }

    if (state == GameState::PAUSE) {
        PauseAction action = view_.hitTestPause(clickPos);
        if (action == PauseAction::Resume) {
            model_.setState(GameState::PLAYING);
            model_.restartSpawnClock();
            frameClock_.restart();
        } else if (action == PauseAction::Menu) {
            model_.setState(GameState::MENU);
        }
    }
}

void GameController::handleMouseMoved(const sf::Event::MouseMoved& moveEvent) {
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(moveEvent.position);
    view_.updateHover(mousePos);
}

void GameController::update() {
    if (model_.getState() != GameState::PLAYING) {
        return;
    }

    float dt = frameClock_.restart().asSeconds();
    const sf::Font& font = view_.fontAt(model_.getFontIndex());
    model_.update(dt, font, static_cast<unsigned>(model_.getCharacterSize()));
}

void GameController::render() {
    window_.clear();
    view_.draw(window_, model_);
    window_.display();
}

} // namespace game
