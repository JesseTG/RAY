#include "Constants.hpp"

namespace ray {

float Constants::ENEMY_RADIUS = -1.f;
float Constants::SEEK_THRESHOLD = -1.f;
int Constants::ENEMY_MOVEMENT_FORCE = -1;
int Constants::ENEMY_MOVE_DURATION = -1;
int Constants::ENEMY_STOPPING_FORCE = -1;

Constants::~Constants()
{
    //dtor
}

void Constants::loadConstants(LuaContext& lua) {
    ENEMY_RADIUS = lua.readVariable<float>("ENEMY_RADIUS");
    SEEK_THRESHOLD = lua.readVariable<float>("SEEK_THRESHOLD");
    ENEMY_MOVEMENT_FORCE = lua.readVariable<int>("ENEMY_MOVEMENT_FORCE");
    ENEMY_MOVE_DURATION = lua.readVariable<int>("ENEMY_MOVE_DURATION");
    ENEMY_STOPPING_FORCE = lua.readVariable<int>("ENEMY_STOPPING_FORCE");
}
}
