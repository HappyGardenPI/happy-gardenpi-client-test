//
// Created by Antonio Salsi on 25/03/22.
//

#pragma once

#include "constants.hpp"

#include <ftxui/component/component.hpp>  // for MenuEntry, Renderer, Vertical
using namespace ftxui;

#include <functional>
#include <cinttypes>
using namespace std;

namespace hgarden::test
{

//using namespace std;

class Tui
{
    uint8_t oldMenuSelected = 0;
public:

    using OnSelect = function<void(uint8_t)>;

    Tui() = default;
    HGARDENPI_NO_COPY_NO_MOVE(Tui)

    void print(const OnSelect&& onSelected = [](uint8_t) {}) const noexcept;

private:
    [[nodiscard]] Element printContainer(const Element&& child) const noexcept;
};

}
