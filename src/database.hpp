//
// Created by Antonio Salsi on 27/01/22.
//

#pragma once

#include <SQLiteCpp/Database.h>

#include "constants.hpp"

namespace hgarden::test
{

namespace sl = SQLite;

class Database
{
    sl::Database database;
public:
    Database() noexcept;
    HGARDENPI_NO_COPY_NO_MOVE(Database)

};

}


