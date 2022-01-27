//
// Created by Antonio Salsi on 27/01/22.
//

#pragma once

//generic no copy no move constructor
#define HGARDENPI_NO_COPY_NO_MOVE(clazz)      \
    clazz(clazz &) = delete;                  \
    clazz &operator=(const clazz &) = delete; \
    clazz(clazz &&) = delete;                 \
    clazz &operator=(clazz &&) = delete;

namespace hgarden::test
{

inline constexpr char * DB_PATH = "./hgarden_client.db";

}