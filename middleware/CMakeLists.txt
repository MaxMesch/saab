cmake_minimum_required(VERSION 3.24)
project(saab_middleware LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

# Standalone (non-Boost), header-only ASIO, downloaded at configure time.
include(FetchContent)
FetchContent_Declare(asio
    URL https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-39-0.tar.gz
    URL_HASH SHA256=cc27087fad4b6d85c6b8e4c896c57144b417cfc5a5eab9e204565f51adaec1de
)
FetchContent_MakeAvailable(asio)

add_executable(saab_middleware
    src/main.cpp
    src/SaabObject.cpp
    src/ObjectStore.cpp
    src/Protocol.cpp
    src/SaabServerClient.cpp
    src/StdoutReporter.cpp
)

target_include_directories(saab_middleware PRIVATE
    ${asio_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(saab_middleware PRIVATE
    ASIO_STANDALONE
    ASIO_NO_DEPRECATED
)

target_link_libraries(saab_middleware PRIVATE Threads::Threads)

target_compile_options(saab_middleware PRIVATE -Wall -Wextra -Wpedantic)
