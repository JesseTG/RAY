#include "ai.hpp"
#include "components.hpp"

#include <LuaContext.hpp>

#include <iostream>
#include <constants.hpp>

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

        if (direction.Length() > Constants::SEEK_THRESHOLD) {
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
        if (direction.Length() <= Constants::SEEK_THRESHOLD) {
            aic.update = &seek;
            return e;
        }

        float vecX = pbc.body->GetLinearVelocity().x;
        float vecY = pbc.body->GetLinearVelocity().y;
        if (!aic.isStationary && aic.moveStepsX == 0 && aic.moveStepsY == 0 && (vecX != 0 || vecY != 0)) {
            if (vecX > 0) {
                vecX = -1 * Constants::ENEMY_STOPPING_FORCE;
            } else if (vecX < 0) {
                vecX = Constants::ENEMY_STOPPING_FORCE;
            } else {
                vecX = 0;
            }
            if (vecY > 0) {
                vecY = -1 * Constants::ENEMY_STOPPING_FORCE;
            } else if (vecY < 0) {
                vecY = Constants::ENEMY_STOPPING_FORCE;
            } else {
                vecY = 0;
            }
            pbc.body->ApplyForceToCenter(b2Vec2(vecX, vecY), true);
            if (std::abs(pbc.body->GetLinearVelocity().x) < 2 && std::abs(pbc.body->GetLinearVelocity().y) < 2) {
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
            if (rand() % 2 == 0) {
                aic.cancelX = false;
            } else {
                aic.cancelX = true;
            }
            aic.moveStepsX = Constants::ENEMY_MOVE_DURATION;
            aic.isStationary = false;
        } else {
            if (!aic.cancelX) {
                float xForce = Constants::ENEMY_MOVEMENT_FORCE;
                if (!aic.isMovingRight) {
                    xForce = xForce * -1;
                }
                if (!aic.cancelY) {
                    xForce = xForce / sqrt(2);
                }
                pbc.body->ApplyForceToCenter(b2Vec2(xForce, 0), true);
            }
            aic.moveStepsX--;
        }
        if (aic.moveStepsY == 0) {
            if (rand() % 2 == 0) {
                aic.isMovingDown = true;
            } else {
                aic.isMovingDown = false;
            }
            if (rand() % 2 == 0) {
                aic.cancelY = false;
            } else {
                aic.cancelY = true;
            }
            aic.moveStepsY = Constants::ENEMY_MOVE_DURATION;
            aic.isStationary = false;
        } else {
            if (!aic.cancelY) {
                float yForce = Constants::ENEMY_MOVEMENT_FORCE;
                if (!aic.isMovingDown) {
                    yForce = yForce * -1;
                }
                if (!aic.cancelX) {
                    yForce = yForce / sqrt(2);
                }
                pbc.body->ApplyForceToCenter(b2Vec2(0, yForce), true);
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
