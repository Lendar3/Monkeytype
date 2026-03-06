# MonkeyTyper

A small typing game built with SFML. Words slide across the screen and you must type them before they escape.

## Features
- Menu, settings, leaderboard, pause, and game-over screens
- Adjustable font and font size
- Adjustable word spawn interval
- Increasing word speed as score grows
- Background switching (Alt+4 / Alt+5)

## Controls
- Type letters to match words
- Backspace: delete last typed character
- Up/Down arrows: decrease/increase spawn interval
- Esc: pause
- Alt+4 / Alt+5: switch background

## Build
Requirements:
- CMake 3.5+
- C++20 compiler
- Internet access on first configure (FetchContent pulls SFML)

```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/Task1
```

## Project Structure
- `src/` MVC implementation and entry point
- `include/` headers
- `assemblers/` assets (fonts, images, word list, leaderboard)

## License
MIT License. See `LICENSE`.
