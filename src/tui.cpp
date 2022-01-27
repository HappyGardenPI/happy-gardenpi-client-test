//
// Created by Antonio Salsi on 27/01/22.
//

#include "tui.hpp"

#if defined _WIN32
#include <conio.h>
#endif

#include <iostream>
using namespace std;

using namespace hgarden::test;

void Tui::print() const noexcept
{
    cout << "Select action" << endl;
    cout << "1: Insert/Update server data" << endl;
}
void Tui::clear() const noexcept
{
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences
#elif defined (__APPLE__)
    system("/usr/bin/clear");
#endif
}
void Tui::setOnMenuSelected(function<void(int)> onMenuSelected) const noexcept
{
    char letter[2];
    cin >> letter;
    cout << letter << endl;
}
