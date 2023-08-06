cmake_minimum_required(VERSION 3.0.0)
project(Needle VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Needle src/main.cpp src/solver.cpp)
include_directories(./src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SFML REQUIRED COMPONENTS graphics window system)
include_directories(${SFML_INCLUDE_DIR})

target_link_libraries(Needle sfml-graphics sfml-system)

include(CPack)
