cmake_minimum_required(VERSION 3.20)

project(game LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin")
set(BUILD_DIR "${CMAKE_BINARY_DIR}")

file(MAKE_DIRECTORY "${BIN_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(
        -g
        -Wall
        -Wextra
        -fsanitize=address,undefined
    )
    add_link_options(-fsanitize=address,undefined)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_library(glad STATIC
    "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c"
)

target_include_directories(glad PUBLIC
    "${CMAKE_SOURCE_DIR}/lib/glad/include"
)

add_executable(game ${SRC_FILES})

target_include_directories(game PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/lib/glad/include"
    ${GLFW3_INCLUDE_DIRS}
)

target_link_directories(game PRIVATE
    ${GLFW3_LIBRARY_DIRS}
)

target_link_libraries(game PRIVATE
    glad
    ${GLFW3_LIBRARIES}
)

target_compile_options(game PRIVATE
    ${GLFW3_CFLAGS_OTHER}
)

if(APPLE)
    target_link_libraries(game PRIVATE
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework Cocoa"
        "-framework QuartzCore"
        "-framework Metal"
    )
endif()

if(UNIX AND NOT APPLE)
    target_link_libraries(game PRIVATE
        dl
        pthread
    )
endif()