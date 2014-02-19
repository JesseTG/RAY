cmake_minimum_required(VERSION 2.6)

project(RAY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_COMPILER clang++)
set(SRC_FILES
    src/entities.cpp
    src/util.cpp
    src/fsm/EntityState.cpp
    src/fsm/EntityStateMachine.cpp
    src/systems/FourWayControlSystem.cpp
    src/systems/MouseFollowControlSystem.cpp
    src/systems/MovementSystem.cpp
    src/systems/PlayerGunSystem.cpp
    src/systems/RenderSystem.cpp
)
set(INCLUDE_FILES
    include/config.hpp
    include/components.hpp
    include/entities.hpp
    include/fsm.hpp
    include/systems.hpp
    include/util.hpp
    include/components/BulletComponent.hpp
    include/components/FourWayControlComponent.hpp
    include/components/MouseFollowControlComponent.hpp
    include/components/PositionComponent.hpp
    include/components/RenderableComponent.hpp
    include/components/VelocityComponent.hpp
    include/fsm/EntityState.hpp
    include/fsm/EntityStateMachine.hpp
    include/systems/FourWayControlSystem.hpp
    include/systems/MouseFollowControlSystem.hpp
    include/systems/MovementSystem.hpp
    include/systems/PlayerGunSystem.hpp
    include/systems/RenderSystem.hpp
)
add_definitions(
    -std=c++11
)

include_directories(
    include
    include/components
    include/fsm
    include/systems
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
