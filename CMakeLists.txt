cmake_minimum_required(VERSION 3.10.0)
project(SpaceShooter VERSION 0.1.0 LANGUAGES C CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
set(target ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# if(MSVC)
#     add_compile_options(/W4)
# else()
#     add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

aux_source_directory(./src SRC)
message(STATUS "find src files: ${SRC}")
file(GLOB header_files ./include/*.hpp)
message(STATUS "find header files: ${header_files}")

add_executable(${target} ${SRC} ${header_files})
target_include_directories(${target} PRIVATE include)
target_link_libraries(${target} PRIVATE
    ${SDL2_LIBRARIES}
    SDL2_mixer::SDL2_mixer
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf)
target_compile_features(${target} PRIVATE cxx_std_17)
target_precompile_headers(${target} PRIVATE ./include/pch.hpp)
