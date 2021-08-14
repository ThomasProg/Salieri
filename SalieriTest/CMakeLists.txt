file(GLOB SOURCES "src/*.cpp" "src/*/*.cpp")

set(CMAKE_BUILD_TYPE Debug)

add_executable(SalieriTest ${SOURCES})

target_link_libraries(SalieriTest PUBLIC Salieri)

target_include_directories(Salieri PUBLIC "${PROJECT_SOURCE_DIR}/Salieri/include")