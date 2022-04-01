//
// Created by Antonio Salsi on 25/03/22.
//

#include <memory>
#include <cstdio>
#include <iostream>
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

void Tui::print(const OnSelect &&onSelected) const noexcept
{
    using namespace ftxui;
    auto screen = ScreenInteractive::TerminalOutput();

    std::vector<std::string> left_menu_entries = {
        "0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%",
    };
    std::vector<std::string> right_menu_entries = {
        "0%", "1%", "2%", "3%", "4%", "5%", "6%", "7%", "8%", "9%", "10%",
    };

    MenuOption menuOption =
        {
            .on_enter = screen.ExitLoopClosure()
        };

    int menuSelected = 0;
    Component leftMenu = Menu(&left_menu_entries, &menuSelected, &menuOption);

    Component&& container = Container::Horizontal({ leftMenu });

    auto renderer = Renderer(container, [&]
    {
        return printContainer(
            hbox({
                     // -------- Left Menu --------------
                     vbox({
                              separator(),
                              hcenter(bold(text("Percentage by 10%"))),
                              separator(),
                              leftMenu->Render(),
                          }),

                     separator(),
                 })
        );
    });

    screen.Loop(renderer);

    if(oldMenuSelected != menuSelected)
    {
        onSelected(menuSelected);
    }

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
                                            separator(),
                                              center(child),
                                              separator()
                                          })
                                 )
                             })
                    )
                )
            )
        )
    ) | border;

}
