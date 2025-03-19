cmake_minimum_required(VERSION 3.30)
project(NetworkEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(MAGNUM_WITH_GLM ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_imguiContext ON CACHE BOOL "" FORCE)

include(FetchContent)

# YAML-CPP
FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG master
)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
)
FetchContent_MakeAvailable(glm)

link_libraries(glm)

set(GLM_INCLUDE_DIR ${glm_SOURCE_DIR}/glm)
	
add_subdirectory(externals)
find_package(Magnum REQUIRED
        GL
        GlfwApplication
        Primitives
        Shaders
        Trade
        MeshTools
        Primitives
        SceneGraph)
			

find_package(MagnumIntegration REQUIRED ImGui Bullet Glm)
find_package(Bullet REQUIRED Dynamics)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)



# Disable contrib and tools
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Disable contrib in yaml-cpp")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable tools in yaml-cpp")

# Configure yaml-cpp to build only the static library (without contrib and tools)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library for yaml-cpp")
FetchContent_MakeAvailable(yaml-cpp)


# Define custom configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release Shipping CACHE STRING "Available build types" FORCE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Shipping" CACHE STRING "Choose the type of build" FORCE)
endif()

# Compiler and linker flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_SHIPPING "/O2 /DNDEBUG /DSHIPPING_BUILD" CACHE STRING "Flags for Shipping configuration" FORCE)
    set(CMAKE_C_FLAGS_SHIPPING "/O2 /DNDEBUG /DSHIPPING_BUILD" CACHE STRING "Flags for Shipping configuration" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_SHIPPING "/O2 /DNDEBUG /DSHIPPING_BUILD" CACHE STRING "Linker flags for Shipping configuration" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_SHIPPING "/O2 /DNDEBUG /DSHIPPING_BUILD" CACHE STRING "Linker flags for Shipping configuration" FORCE)
else()
    set(CMAKE_CXX_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD" CACHE STRING "Flags for Shipping configuration" FORCE)
    set(CMAKE_C_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD" CACHE STRING "Flags for Shipping configuration" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD" CACHE STRING "Linker flags for Shipping configuration" FORCE)
    set(CMAKE_SHARED_LINKER_FLAGS_SHIPPING "-O2 -DNDEBUG -DSHIPPING_BUILD" CACHE STRING "Linker flags for Shipping configuration" FORCE)
endif()

# Set global definitions for all configurations
add_compile_definitions(
    $<$<CONFIG:Shipping>:SHIPPING>
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
)
file(WRITE "${CMAKE_BINARY_DIR}/generated/source_dir.h" "#pragma once\n constexpr const char * SOURCE_DIR = \"${CMAKE_SOURCE_DIR}\";")

include_directories("${CMAKE_BINARY_DIR}/generated")

include_directories(${glm_SOURCE_DIR})


add_subdirectory(NetworkEngine)
add_subdirectory(Game)
add_subdirectory(Editor)

target_link_libraries(NetworkEngine PRIVATE
        Magnum::GlfwApplication 
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::SceneGraph
        Magnum::Shaders
        Magnum::Trade
        MagnumIntegration::Bullet
        Bullet::Dynamics
        MagnumIntegration::ImGui
        MagnumIntegration::Glm
        BulletDynamics
        BulletCollision
        LinearMath
        Bullet3Common)
		
target_link_libraries(Game PRIVATE
        Magnum::GlfwApplication 
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::SceneGraph
        Magnum::Shaders
        Magnum::Trade
        MagnumIntegration::Bullet
        Bullet::Dynamics
        MagnumIntegration::ImGui
		MagnumIntegration::Glm
        BulletDynamics
        BulletCollision
        LinearMath
        Bullet3Common)
		
target_link_libraries(Editor PRIVATE
        Magnum::GlfwApplication 
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::SceneGraph
        Magnum::Shaders
        Magnum::Trade
        MagnumIntegration::Bullet
        Bullet::Dynamics
        MagnumIntegration::ImGui
        MagnumIntegration::Glm
        BulletDynamics
        BulletCollision
        LinearMath
        Bullet3Common)
