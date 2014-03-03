#ifndef SYSTEMS_HPP
#define SYSTEMS_HPP

namespace ray {
    #ifdef DEBUG
    class DebugSystem;
    #endif //DEBUG

    class EntityFollowSystem;
    class FaceEntitySystem;
    class FourWayControlSystem;
    class MouseFollowControlSystem;
    class MovementSystem;
    class PhysicsSystem;
    class PlayerGunSystem;
    class RenderSystem;
    class TractorBeamSystem;
}
#ifdef DEBUG
#include "DebugSystem.hpp"
#endif //DEBUG

#include "EntityFollowSystem.hpp"
#include "FaceEntitySystem.hpp"
#include "FourWayControlSystem.hpp"
#include "MovementSystem.hpp"
#include "MouseFollowControlSystem.hpp"
#include "PhysicsSystem.hpp"
#include "PlayerGunSystem.hpp"
#include "RenderSystem.hpp"
#include "TractorBeamSystem.hpp"

#endif // SYSTEMS_HPP
