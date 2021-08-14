file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp" "src/*/*/*.cpp")

# Packages
find_package(Threads)

set(CMAKE_BUILD_TYPE Debug)

add_library(Salieri ${SOURCES})

target_link_libraries(Salieri ${CMAKE_THREAD_LIBS_INIT})
# target_include_directories(Salieri PUBLIC "${PROJECT_SOURCE_DIR}/Salieri/include")