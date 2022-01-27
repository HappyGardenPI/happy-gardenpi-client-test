cmake_minimum_required(VERSION 3.21)
project(happy_gardenpi_client_test VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
message(STATUS " start SQLiteCpp download")
FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
        GIT_TAG 3.1.1)
FetchContent_MakeAvailable(SQLiteCpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/happy-gardenpi-protocol)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/libmosquitto)

add_executable(happy_gardenpi_client_test
        src/main.cpp
        src/tui.cpp src/tui.hpp src/database.cpp src/database.hpp src/constants.hpp)

target_link_libraries(happy_gardenpi_client_test
        SQLiteCpp
        sqlite3
        libmosquitto
        hgardenpi_protocol
        )