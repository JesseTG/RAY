#ifndef TRACTORBEAMREPELLINGLISTENER_HPP
#define TRACTORBEAMREPELLINGLISTENER_HPP

#include <unordered_map>
#include <utility>

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>

namespace ray {
using std::unordered_multimap;
using std::make_pair;
using std::pair;
using anax::Entity;

/**
 * When two @c b2Fixtures collide, TractorBeamRepellingListener will check to
 * see if one of them is a tractor beam, and pushes the other entity away if so.
 *
 * Maintains a list of @c b2Fixtures that are caught within a tractor beam.
 */
class TractorBeamRepellingListener : public b2ContactListener
{
    public:
        TractorBeamRepellingListener();
        ~TractorBeamRepellingListener();

        /**
         * Detects whether or not the involved fixtures are tractor beams, and
         * adds them to a map of fixtures to process in the TractorBeamSystem
         * later.
         */
        void BeginContact(b2Contact* contact) override;

        /**
         * Removes any fixtures that left a tractor beam from the processing map.
         */
        void EndContact(b2Contact* contact) override;

        void clearGrips();

        unordered_multimap<b2Fixture*, b2Fixture*>& getTractorBeamGrips();
    private:
        unordered_multimap<b2Fixture*, b2Fixture*> _grips;

};
}

#endif // TRACTORBEAMREPELLINGLISTENER_HPP
