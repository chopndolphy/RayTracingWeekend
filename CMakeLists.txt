cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_FLAGS_DEBUG "-O2 -Wall -Wextra -pedantic -g")
# set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
set(CMAKE_CXX_FLAGS "-DNDEBUG -O3")

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(RT_MATH ${LIB}/rt_math)

project(ray_tracer VERSION 1.0)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(ray_tracer ${CMAKE_CURRENT_SOURCE_DIR}/src/ray_tracer.cpp ${SRC_FILES})
target_include_directories(ray_tracer PUBLIC ${INCLUDE} ${RT_MATH})