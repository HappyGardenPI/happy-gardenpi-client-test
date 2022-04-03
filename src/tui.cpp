//
// Created by Antonio Salsi on 25/03/22.
//

#include <memory>
using namespace std;

#include <ftxui/dom/elements.hpp>
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include <ftxui/component/component_base.hpp>      // for ComponentBase
#include <ftxui/component/component_options.hpp>   // for MenuEntryOption
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
using namespace ftxui;

#include "tui.hpp"
using hgarden::test::Tui;

// Define a special style for some menu entry.
static MenuEntryOption colored(Color c = Color::White)
{
    return MenuEntryOption{
        .animated_colors.foreground.enabled = true,
        .animated_colors.background.enabled = true,
        .animated_colors.background.active = c,
        .animated_colors.background.inactive = Color::Black,
        .animated_colors.foreground.active = Color::White,
        .animated_colors.foreground.inactive = c
    };
}

void Tui::print(bool brokerSet, const OnMenuSelect &&onSelected) const noexcept
{
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    Strings menuEntries = {
        "Update data broker",
        "Exit"
    };

    if (!brokerSet)
    {
        menuEntries = {
            "Insert data broker",
            "Exit"
        };
    }

    MenuOption menuOption =
        {
            .on_enter = screen.ExitLoopClosure()
        };

    int menuSelected = 0;
    Component leftMenu = Menu(&menuEntries, &menuSelected, &menuOption);

    auto&& container = Container::Horizontal({ leftMenu });

    auto renderer = Renderer(container, [&]
    {
        return printContainer(
            hbox(filler(),
                 vbox({
                          filler(),
                          center(text("Select option")),
                          filler(),
                          leftMenu->Render(),

                  }) | flex,
                  filler()
              )
        );
    });

    screen.Loop(renderer);

    onSelected(static_cast<MenuSelect>(menuSelected));

}

void Tui::printDataBroker(const OnDataBrokerSelect &&onSelected) const noexcept
{
    auto screen = ScreenInteractive::FitComponent();

    Broker::Ptr broker = make_unique<Broker>();

    string portData;

    Component host = Input(&broker->host, "host");
    Component port = Input(&portData, "port");
    Component user = Input(&broker->user, "user");

    InputOption passwordOption;
    passwordOption.password = true;

    Component passwd = Input(&broker->passwd, "passwd", passwordOption);

    auto&& buttons = Container::Horizontal({
                                             Button("Abort", screen.ExitLoopClosure()),
                                             Button("Test", [&] {  }),
                                             Button("Confirm", [&] {  }),
                                         });

    auto&& component = Container::Vertical({
                                             host,
                                             port,
                                             user,
                                             passwd,
                                             buttons
                                         });

    auto renderer = Renderer(component, [&] {
        return printContainer(
            vbox({
                        hbox(text(" Host     : "), host->Render()),
                        hbox(text(" Port     : "), port->Render()),
                        hbox(text(" User     : "), user->Render()),
                        hbox(text(" Password : "), passwd->Render()),
                        buttons->Render()
                    }) | flex
                    );
    });


    screen.Loop(renderer);
}



Element Tui::printContainer(const Element &&child) const noexcept
{
    return vbox(
        bgcolor(
            Color::Blue,
            vbox(
                color(
                    Color::White,
                    vbox(
                        vbox({
                                 window(
                                     text("Happy GardenPi communication test") | hcenter | bold,
                                     // -------- Top panel --------------
                                     vbox({
                                            filler(),
                                            child | flex,
                                            filler()
                                          })
                                 )
                             })
                    )
                )
            )
        )
    ) ;

}
