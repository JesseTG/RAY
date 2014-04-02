cmake_minimum_required(VERSION 2.8)

project(RAY CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cotire)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to 'Debug' as none was specified.")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(EXECUTABLE_NAME PROJECT_NAME-CMAKE_BUILD_TYPE)

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
    include/listeners
    include/managers
    include/misc
    include/systems
    include/util
    /usr/include
    /usr/local/include
    /usr/include/luajit-2.0
)

find_package(LuaJIT)
find_package(Box2D)

find_package(anax REQUIRED)
if (ANAX_FOUND)
    include_directories(${ANAX_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${ANAX_LIBRARIES})
endif()

find_package(SFML 2 REQUIRED graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

find_package(Boost 1.55.0 REQUIRED regex)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})
endif()

add_executable(EXECUTABLE_NAME
    main.cpp
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(PROJECT_NAME-CMAKE_BUILD_TYPE
        PUBLIC DEBUG
    )
else()
    target_compile_definitions(PROJECT_NAME-CMAKE_BUILD_TYPE
        PUBLIC NDEBUG
    )
endif()

target_link_libraries(PROJECT_NAME-CMAKE_BUILD_TYPE
    luajit-5.1
    Box2D
)

if (WIN AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET Release PROPERTY WIN32_EXECUTABLE)
    # So that Release builds on Windows don't pop up a console window
endif()

if (NOT MSVC)
    add_definitions(
        -std=c++11
    )
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#set(CPACK_PACKAGE_VERSION_MAJOR "${myproject_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${myproject_VERSION_MINOR}")
include(CPack)

