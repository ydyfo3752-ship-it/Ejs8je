cmake_minimum_required(VERSION 3.20)
project(explorers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

find_package(CURL REQUIRED)
find_package(gdiplus REQUIRED)

add_executable(explorers main.cpp)

target_link_libraries(explorers PRIVATE
    CURL::libcurl
        gdiplus
            ws2_32
                gdi32
                    user32
                    )