cmake_minimum_required(VERSION 3.10.0)
project(SpaceShooter VERSION 0.1.0 LANGUAGES C CXX)

include("${CMAKE_SOURCE_DIR}/cmake/CopyDLL.cmake")

add_subdirectory(3rdlibs)

set(SDL_ROOT "" CACHE PATH "SDL root directory")
set(SDL_image_ROOT "" CACHE PATH "SDL_image root directory")
set(SDL_ttf_ROOT "" CACHE PATH "SDL_ttf root director")

set(CMAKE_BUILD_TYPE Release)

find_package(SDL2 REQUIRED PATHS "${SDL_ROOT}/cmake")
find_package(SDL2_image REQUIRED PATHS "${SDL_image_ROOT}/cmake")
find_package(SDL2_ttf REQUIRED PATHS "${SDL_ttf_ROOT}/cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})
set(target ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

aux_source_directory(./src SRC)
message(STATUS "find src files: ${SRC}")
file(GLOB header_files ./include/*.hpp)
message(STATUS "find header files: ${header_files}")

add_executable(${target} ${SRC} ${header_files})
target_include_directories(${target} PRIVATE include)
target_link_libraries(${target} PRIVATE vendor)
target_link_libraries(${target} PRIVATE ${SDL2_LIBRARIES}
                                        SDL2_image::SDL2_image
                                        SDL2_ttf::SDL2_ttf)
target_compile_features(${target} PRIVATE cxx_std_17)
target_precompile_headers(${target} PRIVATE ./include/pch.hpp)
target_compile_definitions(${target} PRIVATE "PROJECT_DIR=\"${CMAKE_SOURCE_DIR}\"")

CopyDLL(${target} "SDL2.dll")
CopyDLL(${target} "SDL2_image.dll")
CopyDLL(${target} "SDL2_ttf.dll")
