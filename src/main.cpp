
#include "tui.hpp"
using hgarden::test::Tui;

#include "database.hpp"
using hgarden::test::Database;

static bool run = true;

int main() {

    Tui tui;
    Database database;

    while (tui.isRun()) {
        tui.print();
        tui.setOnMenuSelected([] (int menuNumber) {

        });
        tui.clear();
    }

    return 0;

}
