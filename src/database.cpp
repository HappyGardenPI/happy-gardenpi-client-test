//
// Created by Antonio Salsi on 27/01/22.
//

#include "database.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

#include "constants.hpp"


namespace hgarden::test
{

static void createDatabase(sql::Database &database) noexcept try
{
    database.exec("CREATE TABLE broker ("
                  " id INTEGER NOT NULL UNIQUE, "
                  " host TEXT NOT NULL, "
                  " port INTEGER NOT NULL DEFAULT 1883, "
                  " user TEXT NOT NULL, "
                  " passwd TEXT NOT NULL, "
                  " PRIMARY KEY(id AUTOINCREMENT) "
                  ");"
    );
}
catch (...)
{
}

hgarden::test::Database::Database() noexcept try :
    tmpFile(move(fs::temp_directory_path().string() + DB_PATH)),
    database(tmpFile, sql::OPEN_READWRITE|sql::OPEN_CREATE)
{

    createDatabase(database);

    // Locking for broker table
    sql::Statement query(database, "SELECT * FROM broker");

    // If not exist I create it
    if (query.executeStep())
    {
        broker = make_unique<Broker>();
        broker->host = move(query.getColumn("host").getString());
        broker->port = query.getColumn("port").getUInt();
        broker->user = move(query.getColumn("user").getString());
        broker->passwd = move(query.getColumn("passwd").getString());
    }
}
catch (...)
{
    exception_ptr e = current_exception();
    cerr << "database exception: " << what() << endl;
}

}