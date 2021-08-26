cmake_minimum_required(VERSION 3.16)
project(happy_gardenpi_client_test VERSION "1.0.0" LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -latomic")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/happy-gardenpi-protocol)

add_executable(happy_gardenpi_client_test
        src/main.cpp)

target_link_libraries(happy_gardenpi_client_test
        pthread
        mosquitto
        hgardenpi_protocol
        )