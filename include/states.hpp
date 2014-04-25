#ifndef STATES_HPP_INCLUDED
#define STATES_HPP_INCLUDED

namespace ray {
    enum class GameState : uint8_t;
    enum class GameStateTransition : uint8_t;
    struct LevelInfo;
    class InGameState;
    class TitleScreenState;
}

#include "GameStates.hpp"
#include "LevelInfo.hpp"
#include "InGameState.hpp"
#include "TitleScreenState.hpp"

#endif // STATES_HPP_INCLUDED
