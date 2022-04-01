cmake_minimum_required(VERSION 3.21)
project(happy_gardenpi_client_test VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# --- Fetch SQLiteCpp --------------------------------------------------------------
message(STATUS " start SQLiteCpp download")
FetchContent_Declare(SQLiteCpp
        GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
        GIT_TAG 3.1.1)
FetchContent_MakeAvailable(SQLiteCpp)
# ------------------------------------------------------------------------------

# --- Fetch FTXUI --------------------------------------------------------------
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        # Important: Specify a GIT_TAG XXXXX here.
        )

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# ------------------------------------------------------------------------------

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/happy-gardenpi-protocol)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/libmosquitto)

add_executable(happy_gardenpi_client_test
        src/main.cpp
        src/database.cpp src/database.hpp src/constants.hpp src/tui.cpp src/tui.hpp)

target_link_libraries(happy_gardenpi_client_test
        PRIVATE SQLiteCpp
        PRIVATE sqlite3
        PRIVATE libmosquitto
        PRIVATE hgardenpi_protocol
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
        )