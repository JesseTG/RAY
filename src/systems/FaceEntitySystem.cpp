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

const ComponentFilter FaceEntitySystem::FILTER = ComponentFilter()
        .requires<FaceEntityComponent>()
        .requiresOneOf<PhysicsBodyComponent>();


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
            PhysicsBodyComponent& pbc = e.getComponent<PhysicsBodyComponent>();
            PositionComponent& target_pos = fe.target.getComponent<PositionComponent>();

            b2Vec2 difference = pbc.body->GetPosition() - sfVecToB2Vec(target_pos.position);
            float theta = atan2(-difference.y, -difference.x);

            pbc.body->SetTransform(pbc.body->GetPosition(), theta);

        }
    }
}
}
