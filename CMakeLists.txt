cmake_minimum_required(VERSION 3.16)
project(MangoMustard VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(RAYLIB_INCLUDE_DIR raylib.h
    PATHS
        /opt/homebrew/include
        /usr/local/include
)

find_library(RAYLIB_LIBRARY raylib
    PATHS
        /opt/homebrew/lib
        /usr/local/lib
)

if (NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
    message(FATAL_ERROR "raylib not found. Install with Homebrew (brew install raylib) or set RAYLIB_INCLUDE_DIR/RAYLIB_LIBRARY.")
endif ()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/titlescreen.cpp
    src/levelselect.cpp
    src/level1.cpp
    src/hud.cpp
    src/player_controller.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${RAYLIB_LIBRARY})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
