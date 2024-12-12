project("CHIP8_Emulator")
cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")



add_executable(
    Emulator
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ByteManipulator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CPU.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Display.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/InputHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyboardMapper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Memory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Timer.cpp
)

# Add SDL2 to the project
find_package(SDL2 REQUIRED)
target_include_directories(Emulator 
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${SDL_INCLUDE_DIRS}/inc
)

target_link_libraries(Emulator PUBLIC ${SDL2_LIBRARIES})
