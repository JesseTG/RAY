#include "FaceEntitySystem.hpp"
#include "components.hpp"

#include "util.hpp"

#include <iostream>
#include <cmath>

namespace ray {

using std::hypot;
using std::cos;
using std::acos;
using std::atan2;

const ComponentFilter FaceEntitySystem::FILTER = ComponentFilter().
        requires<FaceEntityComponent, PositionComponent, RenderableComponent>();

FaceEntitySystem::FaceEntitySystem() : Base(FILTER)
{
    //ctor
}

FaceEntitySystem::~FaceEntitySystem()
{
    //dtor
}

void FaceEntitySystem::update() {
    for (Entity& e : this->getEntities()) {
        // For each entity available...
        FaceEntityComponent& fe = e.getComponent<FaceEntityComponent>();

        if (fe.target.isValid() && fe.target.hasComponent<PositionComponent>()) {
            // If the entity we're tring to face exists and has a position...
            RenderableComponent& ren = e.getComponent<RenderableComponent>();
            if (ren.transformable) {
                // If the entity doing the facing can be rotated...
                PositionComponent& pos = e.getComponent<PositionComponent>();
                PositionComponent& target_pos = fe.target.getComponent<PositionComponent>();

                Vector2f difference = pos.position - target_pos.position;
                float theta = atan2(-difference.y, -difference.x);
                ren.transformable->setRotation(toDegrees(theta));
            }
        }
    }
}
}
