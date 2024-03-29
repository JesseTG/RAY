#ifndef AICOMPONENT_HPP
#define AICOMPONENT_HPP

#include <functional>
#include <string>
#include <boost/optional.hpp>

#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray {
using std::function;
using std::string;
using boost::optional;
using anax::Entity;

/**
 * A @c Component that allows an @c Entity to do something each frame based on
 * its current state, based on its available @c Components, based on the values
 * within those @c Components, or similar.
 */
struct AIComponent : public anax::Component<AIComponent> {
    typedef function<Entity(Entity)> AIUpdateFunction;
    /**
     * Constructor.
     *
     * Accepts an @c Entity and a @c function<void(Entity&)>, which by
     * default is a no-op.
     *
     * @param update The @c function that will be called upon @c entity every frame.
     */
    AIComponent(const AIUpdateFunction& update);

    /**
     * Will be called on @c entity.
     */
    AIUpdateFunction update;

    bool isMovingRight;
    bool isMovingDown;
    int moveStepsX;
    int moveStepsY;
    bool isStationary;
    bool cancelX;
    bool cancelY;

    static void luaInit(LuaContext& lua);
};

}

#endif // AICOMPONENT_HPP
