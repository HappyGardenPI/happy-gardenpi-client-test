cmake_minimum_required(VERSION 3.5)

project(happy-gardenpi-client-test VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/happy-gardenpi-protocol)
find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

include_directories(/usr/local/Cellar/mosquitto/2.0.10_1/include)
link_directories(/usr/local/Cellar/mosquitto/2.0.10_1/lib)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.hpp
    mainwindow.ui
    maincontroller.cpp
    maincontroller.hpp
)


    qt_add_executable(happy-gardenpi-client-test
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET happy-gardenpi-client-test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

target_link_libraries(happy-gardenpi-client-test PRIVATE
    Qt6::Widgets
    Qt6::Core
    hgardenpi_protocol
    mosquitto
    )

set_target_properties(happy-gardenpi-client-test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(happy-gardenpi-client-test)
endif()
