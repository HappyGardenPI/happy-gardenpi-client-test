//
// Created by Antonio Salsi on 25/03/22.
//

#pragma once

#include "constants.hpp"
#include "database.hpp"

#include <ftxui/component/component.hpp>  // for MenuEntry, Renderer, Vertical
using namespace ftxui;

#include <functional>
#include <cinttypes>
using namespace std;

namespace hgarden::test
{

using Broker = Database::Broker;

/**
 * @brief Classt for TUI management
 */
class Tui
{
public:


    enum class MenuSelect
    {
        DATA_BROKER,
        DATA_EXIT
    };

    /**
     * @brief On menu selection callback
     */
    using OnMenuSelect = function<void(MenuSelect)>;

    /**
     * @brief On menu selection callback
    */
    using OnDataBrokerSelect = function<void(Broker::Ptr)>;

    Tui() = default;
    HGARDENPI_NO_COPY_NO_MOVE(Tui)

    /**
     * @brief Print default menu
     * @param brokerSet true when broker data connection are set
     * @param onSelected pass menu selection callback
     */
    void print(bool brokerSet, const OnMenuSelect&& onSelected = [](MenuSelect) {}) const noexcept;

    void printDataBroker(const OnDataBrokerSelect&& onSelected = [](Broker::Ptr) {}) const noexcept;

private:
    /**
     * @brief Build and print default external container for TUI
     * @param child Content to print
     * @return Element to render
     */
    [[nodiscard]] Element printContainer(const Element&& child) const noexcept;
};

}
