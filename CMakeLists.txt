cmake_minimum_required(VERSION 3.10)

# Set Project Name
project(SalieriTest VERSION 1.0)

# configure_file()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# add the Salieri library
add_subdirectory(Salieri)

add_executable(SalieriTest main.cpp)

target_link_libraries(SalieriTest PUBLIC Salieri)

target_include_directories(Salieri PUBLIC "${PROJECT_SOURCE_DIR}/Salieri/include")
