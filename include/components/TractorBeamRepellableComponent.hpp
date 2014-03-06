#ifndef TRACTORBEAMREPELLABLECOMPONENT_HPP
#define TRACTORBEAMREPELLABLECOMPONENT_HPP

#include <anax/anax.hpp>
#include <LuaContext.hpp>

namespace ray {
/**
 * Tag component indicating that an @c Entity can be repelled with a tractor
 * beam.
 */
struct TractorBeamRepellableComponent : public anax::Component<TractorBeamRepellableComponent>
{
};
}

#endif // TRACTORBEAMREPELLABLECOMPONENT_HPP
