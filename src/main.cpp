
#include "tui.hpp"
using hgarden::test::Tui;
using MenuSelect = Tui::MenuSelect;

#include "database.hpp"
using hgarden::test::Database;
using Broker = Database::Broker;

#include <iostream>
using namespace std;


int main() {

    Database database;

    Tui tui;

    tui.print(database.getBroker() != nullptr, [&tui](MenuSelect selected)
    {
        switch (selected) {
        case MenuSelect::DATA_BROKER:
            tui.printDataBroker([](Broker::Ptr broker)
            {
                if(broker)
                {

                }
            });
            break;
        case MenuSelect::DATA_EXIT:
        default:
            cout << to_string(static_cast<uint8_t>(selected)) << endl;
            break;
        }
        cout << to_string(static_cast<uint8_t>(selected)) << endl;
    });

    return EXIT_SUCCESS;


}
