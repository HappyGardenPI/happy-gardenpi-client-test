
#include "tui.hpp"
using hgarden::test::Tui;

#include "database.hpp"
using hgarden::test::Database;

#include <iostream>
using namespace std;

int main() {

    Database database;

    Tui tui;

    tui.print([](uint8_t selected)
    {
        cout << to_string(selected) << endl;
    });

    return EXIT_SUCCESS;


}
