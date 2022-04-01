//
// Created by Antonio Salsi on 27/01/22.
//

#pragma once

#include <SQLiteCpp/Database.h>
#include <memory>

#include "constants.hpp"

namespace hgarden::test
{

namespace sql = SQLite;
using std::unique_ptr;

class Database
{
public:
    struct Broker {
        typedef unique_ptr<Broker> Ptr;
        string host;
        uint16_t port;
        string user;
        string passwd;
    };

private:
    string tmpFile;
    sql::Database database;
    Broker::Ptr broker = nullptr;
public:
    Database() noexcept;
    HGARDENPI_NO_COPY_NO_MOVE(Database)

    [[nodiscard]] inline const Broker::Ptr &getBroker() const noexcept
    {
        return broker;
    }
};

}


