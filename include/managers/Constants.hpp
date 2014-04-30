#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <LuaContext.hpp>

namespace ray {
class Constants
{
    public:
        Constants() = delete;
        virtual ~Constants();

        static void loadConstants(LuaContext&);
        static float SEEK_THRESHOLD;
        static int ENEMY_MOVEMENT_FORCE;
        static int ENEMY_MOVE_DURATION;
        static int ENEMY_STOPPING_FORCE;
};
}
#endif // CONSTANTS_HPP

