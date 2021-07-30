file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")

# Packages
find_package(Threads)

set(CMAKE_BUILD_TYPE Debug)

add_library(Salieri ${SOURCES})

target_link_libraries(Salieri ${CMAKE_THREAD_LIBS_INIT})