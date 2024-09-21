#pragma once

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include "vectorX.h"

namespace cnt
{
    enum FixedPosition
    {
        top,
        left,
        right,
        bottom,
        _default
    };

    class systemOs
    {
    private:
        void init();
        static BOOL GetConsoleWindowSize(CONSOLE_SCREEN_BUFFER_INFO &CONSOLE_SCREEN_BUFFER_INFO);
        static BOOL GetMousePosition(POINT &point);
        void GetConsoleInfo();
        void GetSystemInfo();

    public:
        systemOs();

        cnt::Vector2 consoleWindowsSize;

        cnt::Vector2 screenSize;
        

        cnt::Vector2 consoleMousePosition;

        cnt::Vector2 mousePosition; 

        cnt::Vector2 fixedConsoleSize;
    };

    systemOs::systemOs()
    {
        init();
    }

    void systemOs::init()
    {
        GetConsoleInfo();
        GetSystemInfo();
    }
	
	cnt::Vector2 getFixedConsoleSize(INPUT_RECORD *ir)
    {
        if (ir->EventType == WINDOW_BUFFER_SIZE_EVENT)
        {
            cnt::Vector2 size;
            size.X = ir->Event.WindowBufferSizeEvent.dwSize.X;
            size.Y = ir->Event.WindowBufferSizeEvent.dwSize.Y;
            return size;
        }
        else
        {
            cnt::Vector2 size;
            size.Zero();
            return size;
        }
    }

	void MoveCursor(int x, int y)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position = {(SHORT)x, (SHORT)y};
        SetConsoleCursorPosition(hConsole, position);
    }

    std::string getConsoleTitle()
    {
        char title[0xFF];
        GetConsoleTitleA(title, 0xFF);
        return title;
    }
    
    BOOL systemOs::GetConsoleWindowSize(CONSOLE_SCREEN_BUFFER_INFO &CONSOLE_SCREEN_BUFFER_INFO)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        return GetConsoleScreenBufferInfo(hConsole, &CONSOLE_SCREEN_BUFFER_INFO);
    }

    BOOL systemOs::GetMousePosition(POINT &point)
    {
        return GetCursorPos(&point);
    }
    
    // Get Information Function

    void systemOs::GetConsoleInfo()
    {
        // Get Console Size
        CONSOLE_SCREEN_BUFFER_INFO CONSOLE_SCREEN_BUFFER_INFO;
        if (GetConsoleWindowSize(CONSOLE_SCREEN_BUFFER_INFO))
        {
            consoleWindowsSize.X = CONSOLE_SCREEN_BUFFER_INFO.dwSize.X;
            consoleWindowsSize.Y = CONSOLE_SCREEN_BUFFER_INFO.dwSize.Y;
        }

        // Get Fixed Console Siz
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        INPUT_RECORD irInBuf[256];
        DWORD cNumRead, i;
        if (ReadConsoleInput(hIn, irInBuf, 256, &cNumRead))
            for (i = 0; i < cNumRead; i++)
                getFixedConsoleSize(&irInBuf[i]);

        // Get Console Mouse Position
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (GetConsoleScreenBufferInfo(hConsole, &CONSOLE_SCREEN_BUFFER_INFO))
        {
            COORD position = CONSOLE_SCREEN_BUFFER_INFO.dwCursorPosition;
            consoleMousePosition.X = position.X;
            consoleMousePosition.Y = position.Y;
        }
        else
        {
            consoleMousePosition.Zero();
        }
    }

    void systemOs::GetSystemInfo()
    {
        // Get Mouse in 'Windows System'
        POINT point;
        if (GetMousePosition(point))
        {
            mousePosition.X = point.x;
            mousePosition.Y = point.y;
        }
    }
}
