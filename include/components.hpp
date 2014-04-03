#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

namespace ray {
    struct EntityFollowComponent;
    struct FaceEntityComponent;
    struct FourWayControlComponent;
    struct MouseFollowControlComponent;
    struct PhysicsBodyComponent;
    struct PhysicsFixtureComponent;
    struct PositionComponent;
    struct RenderableComponent;
    struct VelocityComponent;
    struct TractorBeamComponent;
    struct TractorBeamRepellableComponent;
    struct WanderingComponent;
}

#include "EntityFollowComponent.hpp"
#include "FaceEntityComponent.hpp"
#include "FourWayControlComponent.hpp"
#include "MouseFollowControlComponent.hpp"
#include "PhysicsBodyComponent.hpp"
#include "PhysicsFixtureComponent.hpp"
#include "PositionComponent.hpp"
#include "RenderableComponent.hpp"
#include "VelocityComponent.hpp"
#include "TractorBeamComponent.hpp"
#include "TractorBeamRepellableComponent.hpp"
#include "WanderingComponent.hpp"

#endif // COMPONENTS_HPP
