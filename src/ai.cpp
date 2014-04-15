#include "ai.hpp"
#include "components.hpp"

#include <LuaContext.hpp>

namespace ray {

namespace ai {
Entity follow(Entity e) {
    static const ComponentFilter FILTER = ComponentFilter()
        .requires<EntityFollowComponent, PhysicsBodyComponent>();

    if (FILTER.doesPassFilter(e.getComponentTypeList())) {
        PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
        EntityFollowComponent& efc = e.getComponent<EntityFollowComponent>();

        b2Vec2 targetp = efc.target.getComponent<PhysicsBodyComponent>().body->GetPosition();
        b2Vec2 ep = pbc.body->GetPosition();
        b2Vec2 direction = targetp - ep;
        direction.Normalize();

        pbc.body->SetLinearVelocity(direction);
    }
}

Entity wander(Entity e) {
    static const ComponentFilter FILTER = ComponentFilter()
        .requires<PhysicsBodyComponent>();

    if (FILTER.doesPassFilter(e.getComponentTypeList())) {
        PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
        pbc.body->ApplyForceToCenter(b2Vec2(10, 0), true);
    }
}

void initAIBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    lua.writeVariable("AI", LuaEmptyArray); {
        lua.writeVariable("AI", "Follow", &follow);
        lua.writeVariable("AI", "Wander", &wander);
    }

}

}
}
