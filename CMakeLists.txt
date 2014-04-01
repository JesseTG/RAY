cmake_minimum_required(VERSION 2.8)

project(RAY CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cotire)

set(SRC_FILES
    src/entities/sfml_type_bindings.cpp
    src/entities/anax_type_bindings.cpp
    src/entities/box2d_type_bindings.cpp
    src/entities.cpp
    src/util.cpp
    src/components/EntityFollowComponent.cpp
    src/components/FaceEntityComponent.cpp
    src/components/FourWayControlComponent.cpp
    src/components/PhysicsBodyComponent.cpp
    src/components/PhysicsFixtureComponent.cpp
    src/components/PositionComponent.cpp
    src/components/RenderableComponent.cpp
    src/components/TractorBeamComponent.cpp
    src/components/VelocityComponent.cpp
    src/fsm/EntityState.cpp
    src/fsm/EntityStateMachine.cpp
    src/fsm/WorldStateMachine.cpp
    src/fsm/WorldState.cpp
    src/listeners/TractorBeamRepellingListener.cpp
    src/systems/DebugSystem.cpp
    src/systems/EntityFollowSystem.cpp
    src/systems/FaceEntitySystem.cpp
    src/systems/FourWayControlSystem.cpp
    src/systems/MouseFollowControlSystem.cpp
    src/systems/MovementSystem.cpp
    src/systems/PhysicsSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/TractorBeamSystem.cpp
)

set(INCLUDE_FILES
    include/config.hpp
    include/components.hpp
    include/entities.hpp
    include/fsm.hpp
    include/listeners.hpp
    include/systems.hpp
    include/util.hpp
    include/components/EntityFollowComponent.hpp
    include/components/FaceEntityComponent.hpp
    include/components/FourWayControlComponent.hpp
    include/components/MouseFollowControlComponent.hpp
    include/components/PhysicsBodyComponent.hpp
    include/components/PhysicsFixtureComponent.hpp
    include/components/PositionComponent.hpp
    include/components/RenderableComponent.hpp
    include/components/TractorBeamComponent.hpp
    include/components/TractorBeamRepellableComponent.hpp
    include/components/VelocityComponent.hpp
    include/fsm/EntityState.hpp
    include/fsm/EntityStateMachine.hpp
    include/fsm/WorldStateMachine.hpp
    include/fsm/WorldState.hpp
    include/managers/ResourceManager.hpp
    include/listeners/TractorBeamRepellingListener.hpp
    include/systems/DebugSystem.hpp
    include/systems/EntityFollowSystem.hpp
    include/systems/FaceEntitySystem.hpp
    include/systems/FourWayControlSystem.hpp
    include/systems/MouseFollowControlSystem.hpp
    include/systems/MovementSystem.hpp
    include/systems/PhysicsSystem.hpp
    include/systems/RenderSystem.hpp
    include/systems/TractorBeamSystem.hpp
    include/LuaContext.hpp
)

include_directories(
    include
    include/components
    include/fsm
    include/systems
    include/listeners
    include/managers
    /usr/include
    /usr/local/include
    /usr/include/luajit-2.0
)

find_package(Boost)
find_package(LuaJIT)
find_package(anax)
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

target_compile_definitions(Debug
    PUBLIC DEBUG
)

target_compile_definitions(Release
    PUBLIC NDEBUG
)

target_compile_options(Debug
    PUBLIC -Wall
    PUBLIC -Wno-c++98-compat
)

target_compile_options(Release
    PUBLIC -Wall
    PUBLIC -Wno-c++98-compat
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to 'Debug' as none was specified.")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
     "MinSizeRel" "RelWithDebInfo")
endif()

target_link_libraries(Debug
    anax_d
    luajit-5.1
    Box2D
    ${SFML_LIBRARIES}
)

target_link_libraries(Release
    anax
    luajit-5.1
    Box2D
    ${SFML_LIBRARIES}
)

if (WIN)
    set_property(TARGET Release PROPERTY WIN32_EXECUTABLE)
    # So that Release builds on Windows don't pop up a console window
endif()

if (NOT MSVC)
    add_definitions(
        -std=c++11
    )
endif()
