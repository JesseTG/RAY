cmake_minimum_required(VERSION 2.8)


project(RAY CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(cotire)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   message(STATUS "Setting build type to 'Debug' as none was specified.")
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
   # Set the possible values of build type for cmake-gui
   set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

set(EXECUTABLE_NAME ${PROJECT_NAME}-${CMAKE_BUILD_TYPE})

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
    src/listeners/TractorBeamRepellingListener.cpp
    src/managers/color_table.cpp
    src/managers/GameManager.cpp
    src/managers/ImageManager.cpp
    src/managers/ResourceManager.cpp
    src/managers/ScriptManager.cpp
    src/managers/ShapeManager.cpp
    src/systems/DebugSystem.cpp
    src/systems/EntityFollowSystem.cpp
    src/systems/FaceEntitySystem.cpp
    src/systems/FourWayControlSystem.cpp
    src/systems/MouseFollowControlSystem.cpp
    src/systems/MovementSystem.cpp
    src/systems/PhysicsSystem.cpp
    src/systems/RenderSystem.cpp
    src/systems/TractorBeamSystem.cpp
    src/util/EllipseShape.cpp
    src/util/Logger.cpp
    src/util/PolylineShape.cpp
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
    include/fsm/CompositionWorldState.hpp
    include/fsm/EntityState.hpp
    include/fsm/EntityStateMachine.hpp
    include/fsm/WorldStateMachine.hpp
    include/fsm/WorldState.hpp
    include/managers/ResourceManager.hpp
    include/managers/GameManager.hpp
    include/managers/ImageManager.hpp
    include/managers/ResourceManager.hpp
    include/managers/ScriptManager.hpp
    include/managers/ShapeManager.hpp
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
    include/util/EllipseShape.hpp
    include/util/Logger.hpp
    include/util/PolylineShape.hpp
    include/LuaContext.hpp
    include/misc/exception.hpp
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
)

add_executable(${EXECUTABLE_NAME}
    main.cpp
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

find_package(LuaJIT)
if (LUAJIT_FOUND)
    include_directories(${LUAJIT_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${LUAJIT_LIBRARIES})
else()
    find_package(Lua52)
    if (LUA52_FOUND)
        message(STATUS "LuaJIT not found, using Lua 5.2 instead (we prefer LuaJIT, it's faster)")
        include_directories(${LUA_INCLUDE_DIR})
        target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})
    else()
        message(SEND_ERROR "LuaJIT or Lua 5.2 is required")
    endif()
endif()

find_package(Box2D REQUIRED)
include_directories(${BOX2D_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${BOX2D_LIBRARY})

find_package(Thor 2 REQUIRED)
include_directories(${THOR_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${THOR_LIBRARY})

find_package(anax REQUIRED)
include_directories(${ANAX_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${ANAX_LIBRARIES})

find_package(SFML 2 REQUIRED graphics window system)
include_directories(${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

find_package(Boost 1.55.0 REQUIRED regex)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${Boost_LIBRARIES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${EXECUTABLE_NAME}
        PUBLIC DEBUG
    )
else()
    target_compile_definitions(${EXECUTABLE_NAME}
        PUBLIC NDEBUG
    )
endif()

if (WIN AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_property(TARGET Release PROPERTY WIN32_EXECUTABLE)
    # So that Release builds on Windows don't pop up a console window
endif()

if (NOT MSVC)
    # Only MSVC builds in C++11 mode by default
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

