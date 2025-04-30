cmake_minimum_required(VERSION 3.30)
project(NetworkEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(MAGNUM_WITH_GLM ON CACHE BOOL "" FORCE)
set(MAGNUM_WITH_imguiContext ON CACHE BOOL "" FORCE)

set(BULLET_BUILD_BULLETROBOTICS OFF CACHE BOOL "Disable Bullet Robotics module" FORCE)
set(BULLET_BUILD_BULLETPHYSICS OFF CACHE BOOL "Disable Bullet Robotics GUI" FORCE)
#
## Disable Bullet extras if not needed
#set(BUILD_BULLET2_DEMOS OFF)
#set(BUILD_EXTRAS OFF)                # <-- This disables BulletRoboticsGUI and others
#set(BUILD_UNIT_TESTS OFF)

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

FetchContent_Declare(
    enet
    GIT_REPOSITORY https://github.com/SirLynix/enet6.git
    GIT_TAG main
)
FetchContent_MakeAvailable(enet)


# Disable install/export for libcurl and CPR
set(CMAKE_INSTALL_PREFIX "" CACHE PATH "" FORCE)  # Prevent installation

# Disable shared libraries for both curl and CPR
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(CURL_BUILD_SHARED_LIBS OFF CACHE BOOL "Build static curl" FORCE)
set(CURL_DISABLE_INSTALL ON CACHE BOOL "Disable curl installation" FORCE)
set(CURL_ENABLE_STATIC_LIBS ON CACHE BOOL "Force static curl" FORCE)
set(CURL_ENABLE_MANUAL_CURL_BUILD OFF CACHE BOOL "Prevent manual build" FORCE)

# Prevent building and exporting targets
set(CURL_ENABLE_EXPORT ON CACHE BOOL "Disable curl export" FORCE)

# Disable tests
set(BUILD_TESTING OFF CACHE BOOL "Disable tests" FORCE)
set(CPR_BUILD_TESTS OFF CACHE BOOL "Don't build CPR tests" FORCE)

# Disable export behavior for CPR
set(CMAKE_INSTALL_EXPORT_NAME "" CACHE STRING "" FORCE)

# Declare zlib (if you need it for curl)
FetchContent_Declare(
  zlib
  URL https://zlib.net/zlib-1.3.1.tar.gz
)
FetchContent_MakeAvailable(zlib)
include_directories(${zlib_BINARY_DIR})

# Declare nghttp2 (if needed for HTTP/2 support)
FetchContent_Declare(
  nghttp2
  GIT_REPOSITORY https://github.com/nghttp2/nghttp2.git
  GIT_TAG v1.59.0
)
FetchContent_MakeAvailable(nghttp2)
include_directories(${nghttp2_SOURCE_DIR}/lib)

# Declare CPR
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)
set(ZLIB_INCLUDE_DIR "${zlib_SOURCE_DIR}")
set(NGHTTP2_INCLUDE_DIR ${nghttp2_SOURCE_DIR}/lib/includes)
set(NGHTTP2_LIBRARY nghttp2)
FetchContent_MakeAvailable(cpr)

link_libraries(glm)
link_libraries(enet)

set(GLM_INCLUDE_DIR ${glm_SOURCE_DIR}/glm)
set(ENET_INCLUDE_DIR ${enet_SOURCE_DIR}/include)
	
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
include_directories(${enet_SOURCE_DIR}/include)
include_directories(${cpr_SOURCE_DIR}/include)


add_subdirectory(NetworkEngine)
#add_subdirectory(Game)
add_subdirectory(Editor)
add_subdirectory(Client)
add_subdirectory(Server)

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
        Bullet3Common
		cpr::cpr
		zlib
		nghttp2
)
		
#target_link_libraries(Game PRIVATE
#        Magnum::GlfwApplication 
#        Magnum::GL
#        Magnum::Magnum
#        Magnum::MeshTools
#        Magnum::Primitives
#        Magnum::SceneGraph
#        Magnum::Shaders
#        Magnum::Trade
#        MagnumIntegration::Bullet
#        Bullet::Dynamics
#        MagnumIntegration::ImGui
#		MagnumIntegration::Glm
#        BulletDynamics
#        BulletCollision
#        LinearMath
#        Bullet3Common)
		
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
        Bullet3Common
		cpr::cpr
		zlib
		nghttp2)

target_link_libraries(Client PRIVATE
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
        Bullet3Common
		cpr::cpr
		zlib
		nghttp2)


target_link_libraries(Server PRIVATE
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
        Bullet3Common
		cpr::cpr
		zlib
		nghttp2)
