add_executable(julia_fractal julia_fractal.cpp)

set_property(TARGET julia_fractal PROPERTY CXX_STANDARD 20)

target_link_libraries(julia_fractal PRIVATE ${THIRD_PARTY_LIBRARIES})
target_include_directories(julia_fractal PRIVATE ${THIRD_PARTY_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})