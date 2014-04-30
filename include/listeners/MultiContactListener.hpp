#ifndef MULTICONTACTLISTENER_HPP
#define MULTICONTACTLISTENER_HPP

#include <memory>
#include <vector>
#include <Box2D/Box2D.h>

namespace ray {
using std::unique_ptr;
using std::vector;
class MultiContactListener : public b2ContactListener
{
    public:
        MultiContactListener();
        virtual ~MultiContactListener();

        void addListener(b2ContactListener* listener);

        void clearListeners();

        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
    private:
        vector<b2ContactListener*> _listeners;
};
}

#endif // MULTICONTACTLISTENER_HPP
