cmake_minimum_required(VERSION 2.8)

project(RAY CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

set(SRC_FILES
    src/entities.cpp
    src/util.cpp
    src/fsm/EntityState.cpp
    src/fsm/EntityStateMachine.cpp
    src/systems/EntityFollowSystem.cpp
    src/systems/FaceEntitySystem.cpp
    src/systems/FourWayControlSystem.cpp
    src/systems/MouseFollowControlSystem.cpp
    src/systems/MovementSystem.cpp
    src/systems/PlayerGunSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/TractorBeamSystem.cpp
)

set(INCLUDE_FILES
    include/config.hpp
    include/components.hpp
    include/entities.hpp
    include/fsm.hpp
    include/systems.hpp
    include/util.hpp
    include/components/AccelerationComponent.hpp
    include/components/BoundingVolumeComponent.hpp
    include/components/BulletComponent.hpp
    include/components/EntityFollowComponent.hpp
    include/components/FaceEntityComponent.hpp
    include/components/FourWayControlComponent.hpp
    include/components/MouseFollowControlComponent.hpp
    include/components/PositionComponent.hpp
    include/components/RenderableComponent.hpp
    include/components/TractorBeamComponent.hpp
    include/components/TractorBeamRepellableComponent.hpp
    include/components/VelocityComponent.hpp
    include/fsm/EntityState.hpp
    include/fsm/EntityStateMachine.hpp
    include/systems/EntityFollowSystem.hpp
    include/systems/FaceEntitySystem.hpp
    include/systems/FourWayControlSystem.hpp
    include/systems/MouseFollowControlSystem.hpp
    include/systems/MovementSystem.hpp
    include/systems/PlayerGunSystem.hpp
    include/systems/RenderSystem.hpp
    include/systems/TractorBeamSystem.hpp
)

if (NOT MSVC)
    add_definitions(
        -std=c++11
    )
endif()

if (WIN)
    set_property(GLOBAL PROPERTY WIN32_EXECUTABLE)
endif()

include_directories(
    include
    include/components
    include/fsm
    include/systems
    /usr/include
    /usr/local/include
)

find_package(anax_d)
find_package(Box2D)
find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_executable(Debug
    main.cpp
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

add_executable(Release
    EXCLUDE_FROM_ALL
    main.cpp
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

target_link_libraries(Debug
    anax_d
    Box2D
    ${SFML_LIBRARIES}
)

target_link_libraries(Release
    anax_d
    Box2D
    ${SFML_LIBRARIES}
)
