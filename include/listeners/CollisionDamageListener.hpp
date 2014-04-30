#ifndef COLLISIONDAMAGELISTENER_HPP
#define COLLISIONDAMAGELISTENER_HPP

#include <Box2D/Box2D.h>
#include <anax/anax.hpp>

namespace ray {
class CollisionDamageListener : public b2ContactListener {
    public:
        CollisionDamageListener();
        virtual ~CollisionDamageListener();
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    protected:
    private:
};
}

#endif // COLLISIONDAMAGELISTENER_HPP
