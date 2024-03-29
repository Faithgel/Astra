# Astra

## Installation Guide

### Prerequisites

- CMake (version 3.22 or higher)
- A C++ compiler that supports C++17
- [D++ (Discord++)](https://github.com/brainboxdotcc/DPP) library
- [spdlog](https://github.com/gabime/spdlog) library

### Steps

1. Clone the repository:
```sh
    git clone https://github.com/yourusername/astra.git## Description

Astra is a Discord bot primarily used for server moderation and managing TTRPG (Tabletop Role-Playing Game) plays. It comes with a journal feature for note-taking during games and a dice roller for in-game actions. Future updates will include additional features to enhance the gaming experience.
```

2. Create a build directory:
```sh
    mkdir build
    cd build
```
3. Run CMake:
```sh
    cmake ..
```

4. Build the project:
```sh
    cmake --build .
```
4. set the environment variable in your system
```sh
    export TOKEN_BOT_ASTRA=your_token
```

5. Run the bot:
```sh
    ./build/discord-bot
```
