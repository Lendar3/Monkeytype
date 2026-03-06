#pragma once

namespace game {
constexpr unsigned kWindowWidth = 1280;
constexpr unsigned kWindowHeight = 720;

constexpr const char* kWordsFile = "../assemblers/words.txt";
constexpr const char* kLeaderboardFile = "../assemblers/leaderboard.txt";

constexpr const char* kFontArial = "../assemblers/Fonts/arial.ttf";
constexpr const char* kFontSFFedora = "../assemblers/Fonts/SFFedora.ttf";
constexpr const char* kFontPlayfair = "../assemblers/Fonts/PlayfairDisplay-Regular.ttf";

constexpr const char* kBackground1 = "../assemblers/img/1.jpg";
constexpr const char* kBackground2 = "../assemblers/img/2.jpg";
constexpr const char* kBackground3 = "../assemblers/img/3.jpg";

constexpr int kDefaultFontIndex = 2;
constexpr int kDefaultCharacterSize = 24;
constexpr float kDefaultSpawnInterval = 2.0f;
} // namespace game
