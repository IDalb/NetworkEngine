cmake_minimum_required(VERSION 3.20)
project(Editor VERSION 1.0 LANGUAGES CXX)

# Collect source and header files
file(GLOB_RECURSE EDITOR_SOURCES src/*.cpp)
file(GLOB_RECURSE EDITOR_HEADERS include/*.h)

# Create the executable
add_executable(Editor ${EDITOR_SOURCES} ${EDITOR_HEADERS})

foreach(FILE_PATH ${EDITOR_SOURCES})
    # Get the relative path of the file to the src directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/src" ${FILE_PATH})

    # Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../Editor/" "/" REL_PATH "${REL_PATH}")

    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
    #if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
    #    source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()

foreach(FILE_PATH ${EDITOR_HEADERS})
    # Get the relative path of the file to the include directory
    file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/include" ${FILE_PATH})
    
	# Normalize the path to remove unwanted prefixes like "../"
    string(REPLACE "../Editor/" "/" REL_PATH "${REL_PATH}")

    # Extract the folder hierarchy (relative path without filename)
    get_filename_component(FOLDER_PATH "${REL_PATH}" PATH)

    # Create filters matching the folder hierarchy
   # if(FOLDER_PATH AND NOT FOLDER_PATH MATCHES "^\\.\\.")
        source_group("${FOLDER_PATH}" FILES ${FILE_PATH})
    #else()
     #   source_group("Root" FILES ${FILE_PATH})
    #endif()
endforeach()


# Include directories
target_include_directories(Editor PUBLIC include)

target_include_directories(Editor PUBLIC ${yaml-cpp_SOURCE_DIR}/include)


# Link with BeaverEngine
target_link_libraries(Editor PUBLIC NetworkEngine yaml-cpp)