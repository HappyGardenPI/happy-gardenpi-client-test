//
// Created by Antonio Salsi on 27/01/22.
//

#pragma once

#include <functional>

#include "constants.hpp"

namespace hgarden::test
{

using std::function;

class Tui
{
    bool run = true;
public:
    Tui() = default;
    HGARDENPI_NO_COPY_NO_MOVE(Tui)

    void print() const noexcept;
    void clear() const noexcept;

    void setOnMenuSelected(function<void(int)> onMenuSelected) const noexcept;

    [[nodiscard]] inline bool isRun() const noexcept
    {
        return run;
    }
};

}


