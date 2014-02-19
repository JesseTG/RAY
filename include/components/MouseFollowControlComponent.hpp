#ifndef MOUSEFOLLOWCONTROLCOMPONENT_HPP
#define MOUSEFOLLOWCONTROLCOMPONENT_HPP

#include <anax/anax.hpp>

namespace ray
{
using anax::Component;

/**
 * Enables an @c Entity's position to be directly controlled by the mouse.
 *
 * This is just a tag @c Component, i.e. it does not actually store any data.
 */
struct MouseFollowControlComponent : public Component<MouseFollowControlComponent>
{
};
}

#endif // MOUSEFOLLOWCONTROLCOMPONENT_HPP
