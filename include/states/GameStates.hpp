#ifndef GAMESTATES_HPP_INCLUDED
#define GAMESTATES_HPP_INCLUDED

namespace ray {
enum class GameState : uint8_t {
    SPLASH,
    TITLE,
    OPTIONS,
    ABOUT,
    IN_GAME,
    HIGH_SCORE,
};

enum class GameStateTransition : uint8_t {
    ADVANCE,
    RETURN,
    RELOAD,
};
}


#endif // GAMESTATES_HPP_INCLUDED
