#pragma once

#include <iostream>
#include <conio.h>
#include <string>

#include "systemOs.h"
#include "vectorX.h"
#include "listenEvents.h"

namespace cnt
{
    std::string input(std::string PreSymbol = "")
    {
        std::cout << PreSymbol;
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    std::string password_input(std::string PreSymbol = "")
    {
        std::string input;
        char ch;
        while ((ch = _getch()) != '\r')
        {
            if (ch != '\b')
                input += ch;
            else if (!input.empty())
            {
                input.pop_back();
                std::cout << "\b \b";
            }
        }
        return input;
    }

    std::string CustomInput(std::string PreSymbol = "", bool canSee = true, char cantSeeModeCharacter = '\0', cnt::FixedPosition position = cnt::_default, bool Adaptive = false)
    {
        cnt::systemOs os;
        std::string input;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        bool isVertical = false;
        

        // Output
        std::cout << PreSymbol;
        if (position == cnt::FixedPosition::left || position == cnt::FixedPosition::right)
            std::cout << std::endl;

        while (true){
            // Move
            switch (position)
            {
                case cnt::FixedPosition::top:
                    cnt::MoveCursor(0, 0);
                    break;
                case cnt::FixedPosition::bottom:
                    cnt::MoveCursor(0, os.consoleWindowsSize.Y - 1);
                    break;
                case cnt::FixedPosition::left:
                    cnt::MoveCursor(0, 0);
                    isVertical = true;
                    break;
                case cnt::FixedPosition::right:
                    cnt::MoveCursor(os.consoleWindowsSize.X - 1, 0);
                    isVertical = true;
                    break;
                default:
                    break;
            }

            if(kbhit()){
                char Key = getch();
                if (Key != '\r'){
                    if (Key != '\b')
                    {
                        input += Key;
                        if (canSee)
                        {
                            if (isVertical)
                                std::cout << std::endl << Key;
                            else 
                                std::cout << Key;
                        }
                        else
                        {
                            std::cout << cantSeeModeCharacter;
                        }
                    }
                    else if (!input.empty())
                    {
                        input.pop_back();
                        std::cout << "\b \b";
                    }
                }
                else
                    return input;
            }
        }
    }
}