cmake_minimum_required(VERSION 3.5)

project(happy-gardenpi-client-test VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3thparty/happy-gardenpi-protocol)

add_executable(happy-gardenpi-client-test
        main.cpp
        maincontroller.cpp
        maincontroller.hpp
        3thparty/mosquitto/actions.c
        3thparty/mosquitto/alias_mosq.c
        3thparty/mosquitto/alias_mosq.h
        3thparty/mosquitto/callbacks.c
        3thparty/mosquitto/config.h
        3thparty/mosquitto/connect.c
        3thparty/mosquitto/deps
        3thparty/mosquitto/dummypthread.h
        3thparty/mosquitto/handle_auth.c
        3thparty/mosquitto/handle_connack.c
        3thparty/mosquitto/handle_disconnect.c
        3thparty/mosquitto/handle_ping.c
        3thparty/mosquitto/handle_pubackcomp.c
        3thparty/mosquitto/handle_publish.c
        3thparty/mosquitto/handle_pubrec.c
        3thparty/mosquitto/handle_pubrel.c
        3thparty/mosquitto/handle_suback.c
        3thparty/mosquitto/handle_unsuback.c
        3thparty/mosquitto/helpers.c
        3thparty/mosquitto/linker.version
        3thparty/mosquitto/logging_mosq.c
        3thparty/mosquitto/logging_mosq.h
        3thparty/mosquitto/loop.c
        3thparty/mosquitto/memory_mosq.c
        3thparty/mosquitto/memory_mosq.h
        3thparty/mosquitto/messages_mosq.c
        3thparty/mosquitto/messages_mosq.h
        3thparty/mosquitto/misc_mosq.c
        3thparty/mosquitto/misc_mosq.h
        3thparty/mosquitto/mosquitto.c
        3thparty/mosquitto/mosquitto.h
        3thparty/mosquitto/mosquitto_internal.h
        3thparty/mosquitto/mqtt_protocol.h
        3thparty/mosquitto/net_mosq.c
        3thparty/mosquitto/net_mosq.h
        3thparty/mosquitto/net_mosq_ocsp.c
        3thparty/mosquitto/options.c
        3thparty/mosquitto/packet_datatypes.c
        3thparty/mosquitto/packet_mosq.c
        3thparty/mosquitto/packet_mosq.h
        3thparty/mosquitto/property_mosq.c
        3thparty/mosquitto/property_mosq.h
        3thparty/mosquitto/read_handle.c
        3thparty/mosquitto/read_handle.h
        3thparty/mosquitto/send_connect.c
        3thparty/mosquitto/send_disconnect.c
        3thparty/mosquitto/send_mosq.c
        3thparty/mosquitto/send_mosq.h
        3thparty/mosquitto/send_publish.c
        3thparty/mosquitto/send_subscribe.c
        3thparty/mosquitto/send_unsubscribe.c
        3thparty/mosquitto/socks_mosq.c
        3thparty/mosquitto/socks_mosq.h
        3thparty/mosquitto/srv_mosq.c
        3thparty/mosquitto/strings_mosq.c
        3thparty/mosquitto/thread_mosq.c
        3thparty/mosquitto/time_mosq.c
        3thparty/mosquitto/time_mosq.h
        3thparty/mosquitto/tls_mosq.c
        3thparty/mosquitto/tls_mosq.h
        3thparty/mosquitto/utf8_mosq.c
        3thparty/mosquitto/util_mosq.c
        3thparty/mosquitto/util_mosq.h
        3thparty/mosquitto/util_topic.c
        3thparty/mosquitto/will_mosq.c
        3thparty/mosquitto/will_mosq.h
        )



