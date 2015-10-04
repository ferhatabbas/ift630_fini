cmake_minimum_required(VERSION 2.6)
project(tp3)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_system -lpthread -std=c++11")
set(tp3srv src/server.cpp)
set(tp3client src/client.cpp)

include_directories(${Boost_INCLUDE_DIR})

add_executable(tp3srv ${tp3srv})
target_link_libraries(tp3srv ${Boost_SYSTEM_LIBRARY})

add_executable(tp3client ${tp3client})
target_link_libraries(tp3srv ${Boost_SYSTEM_LIBRARY})
