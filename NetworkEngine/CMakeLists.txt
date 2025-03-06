cmake_minimum_required(VERSION 3.20)
project(NetworkEngine VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS include/*.h)

foreach(FILE_PATH ${ENGINE_SOURCES})
    # Get the relative path of the file to the src directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/src" ${FILE_PATH})

    # Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../NetworkEngine/" "/" REL_PATH "${REL_PATH}")

    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
    #if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
    #    source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()

foreach(FILE_PATH ${ENGINE_HEADERS})
    # Get the relative path of the file to the include directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/include" ${FILE_PATH})

    # Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../NetworkEngine/" "/" REL_PATH "${REL_PATH}")
	
    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
   # if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
     #   source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()

# Create the library
add_library(NetworkEngine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Include directories
target_include_directories(NetworkEngine PUBLIC include)

# Include the yaml-cpp headers
target_include_directories(NetworkEngine PUBLIC ${yaml-cpp_SOURCE_DIR}/include)


# Link external libraries
target_link_libraries(NetworkEngine
    PUBLIC
        yaml-cpp
)