#include "ai.hpp"
#include "components.hpp"

#include <LuaContext.hpp>

#include <iostream>

namespace ray {

namespace ai {
Entity seek(Entity e) {
    static const ComponentFilter FILTER = ComponentFilter()
        .requires<EntityFollowComponent, PhysicsBodyComponent>();

    if (FILTER.doesPassFilter(e.getComponentTypeList())) {
        PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
        EntityFollowComponent& efc = e.getComponent<EntityFollowComponent>();

        b2Vec2 targetp = efc.target.getComponent<PhysicsBodyComponent>().body->GetPosition();
        b2Vec2 ep = pbc.body->GetPosition();
        b2Vec2 direction = targetp - ep;

        if (direction.Length() > 100.f) {
            e.getComponent<AIComponent>().update = &wander;
            return e;
        }

        direction.Normalize();
        direction *= efc.maxSpeed;

        pbc.body->SetLinearVelocity(direction);
    }

    return e;
}

Entity wander(Entity e) {
    static const ComponentFilter FILTER = ComponentFilter()
        .requires<EntityFollowComponent, PhysicsBodyComponent>();

    if (FILTER.doesPassFilter(e.getComponentTypeList())) {
        PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
        EntityFollowComponent& efc = e.getComponent<EntityFollowComponent>();

        b2Vec2 targetp = efc.target.getComponent<PhysicsBodyComponent>().body->GetPosition();
        b2Vec2 ep = pbc.body->GetPosition();
        b2Vec2 direction = targetp - ep;

        AIComponent& aic = e.getComponent<AIComponent>();
        if (direction.Length() <= 100.f) {
            aic.update = &seek;
            return e;
        }

        float vecX = pbc.body->GetLinearVelocity().x;
        float vecY = pbc.body->GetLinearVelocity().y;
        if (!aic.isStationary && aic.moveStepsX == 0 && aic.moveStepsY == 0 && (vecX != 0 || vecY != 0)) {
            if (vecX > 0) {
                vecX = -10;
            } else if (vecX < 0) {
                vecX = 10;
            } else {
                vecX = 0;
            }
            if (vecY > 0) {
                vecY = -10;
            } else if (vecY < 0) {
                vecY = 10;
            } else {
                vecY = 0;
            }
            pbc.body->ApplyForceToCenter(b2Vec2(vecX, vecY), true);
            if (std::abs(pbc.body->GetLinearVelocity().x) < 1 && std::abs(pbc.body->GetLinearVelocity().y) < 1) {
                pbc.body->SetLinearVelocity(b2Vec2(0, 0));
                aic.isStationary = true;
            }
            return e;
        }
        if (aic.moveStepsX == 0) {
            if (rand() % 2 == 0) {
                aic.isMovingRight = true;
            } else {
                aic.isMovingRight = false;
            }
            aic.moveStepsX = 100;
            aic.isStationary = false;
        } else {
            if (aic.isMovingRight) {
                pbc.body->ApplyForceToCenter(b2Vec2(10, 0), true);
            } else {
                pbc.body->ApplyForceToCenter(b2Vec2(-10, 0), true);
            }
            aic.moveStepsX--;
        }
        if (aic.moveStepsY == 0) {
            if (rand() % 2 == 0) {
                aic.isMovingDown = true;
            } else {
                aic.isMovingDown = false;
            }
            aic.moveStepsY = 100;
            aic.isStationary = false;
        } else {
            if (aic.isMovingDown) {
                pbc.body->ApplyForceToCenter(b2Vec2(0, 10), true);
            } else {
                pbc.body->ApplyForceToCenter(b2Vec2(0, -10), true);
            }
            aic.moveStepsY--;
        }
    }

    return e;
}

void initAIBindings(GameManager& game) {
    LuaContext& lua = *game.getLuaContext();

    lua.writeVariable("AI", LuaEmptyArray); {
        lua.writeVariable("AI", "Seek", &seek);
        lua.writeVariable("AI", "Wander", &wander);
    }

}

}
}
