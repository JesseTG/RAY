#ifndef TRACTORBEAMCOMPONENT_HPP
#define TRACTORBEAMCOMPONENT_HPP

#include <anax/anax.hpp>


namespace ray {

/**
 * Lets an @c Entity be used as a tractor beam.
 */
struct TractorBeamComponent : public anax::Component<TractorBeamComponent>
{
    TractorBeamComponent() :
        starting_width(16.0),
        starting_angle(0.0),
        length(256),
        force(0)
    {}

    /**
     * How wide the beam will be at its point of origin, in pixels. For example,
     * if set to zero, the treactor beam will originate from one point.  If set
     * large enough, the point of origin could be larger than the player character is.
     */
    float starting_width;

    /**
     * The angle the tractor beam will extend, in radians. For example, if set
     * to zero, the tractor beam will extend perfectly horizontally. If set to
     * 90 degrees (aka @c pi/2 radians), the the beam will extend perpendicularly
     * to the direction the player is flying.
     */
    float starting_angle;

    /**
     * The length the tractor beam will extend, in pixels. If set to zero, will
     * be disabled. If set to infinity, will extend to the edge of the arena. If
     * negative, will extend in the opposite direction the player intends to fire.
     */
    float length;

    /**
     * The attracting/repelling force (units TBD). Zero means disabled. Positive
     * is attracting. Negative is repelling.
     */
    float force;
};
}

#endif // TRACTORBEAMCOMPONENT_HPP
