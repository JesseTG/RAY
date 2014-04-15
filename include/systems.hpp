#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

namespace ray {
    #ifdef DEBUG
    class DebugSystem;
    #endif //DEBUG

    class AISystem;
    class EntityFollowSystem;
    class FaceEntitySystem;
    class FourWayControlSystem;
    class MouseFollowControlSystem;
    class MovementSystem;
    class PhysicsSystem;
    class RenderSystem;
    class TractorBeamSystem;
}

#ifdef DEBUG
#include "DebugSystem.hpp"
#endif //DEBUG

#include "AISystem.hpp"
#include "EntityFollowSystem.hpp"
#include "FaceEntitySystem.hpp"
#include "FourWayControlSystem.hpp"
#include "MovementSystem.hpp"
#include "MouseFollowControlSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"
#include "TractorBeamSystem.hpp"

#endif // SYSTEMS_HPP
