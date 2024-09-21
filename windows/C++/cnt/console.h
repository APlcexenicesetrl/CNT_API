/*
 * Components for console operations -*- C++ -*-
 *
 * Copyright (C) [Year] [Your Name or Organization]
 *
 * This file is part of the custom console library for C++.
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * see the file COPYING3.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file console.h
 *  This is a custom C++ Library header for console operations.
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include <conio.h>
#include <map>
#include <windows.h>

#include "string.h"

namespace cnt
{
    // Class
    class console
    {
	    private:
	        bool debugSwitchValue_STAR = false;
	        bool debugTellSayState_STAR = true;
	        int IndentBlockNumber = 4;
	        std::map<std::string, int> consoleCountMap;
	        std::vector<std::string> consoleCountList;
	        std::vector<std::string> groupIndentStack;
	        std::vector<std::string> timeNameList;
	        std::string getTimeText();
	        std::string getLogOutText(const std::string &LogReferenceText, const std::string &logOutText);
	        void winLog(std::string logText, std::vector<std::string> &returnList);
	        void debugTellSayState();
	        std::string groupIndentLogOut(const bool noGroupIndentLogOut = false, const int add = 0);
	
	    public:
	        // ConsoleColor
	        void SetConsoleColor(WORD color);
	        void ResetConsoleColor();
	        // Keyboard
	        void waitKey(const char downKey);
	        bool ifDownKey(const char downKey);
	        // Log
	        void winError(std::string SourceError, std::string ErrorDescription, int ErrorCode);
	        void log(std::string logText);
	        void warn(std::string logText);
	        void error(std::string logText);
	        void info(std::string logText);
	        void debug(std::string logText, bool thisStartDebug = false);
	        void staticLog(const std::string logText, bool noGroupIndentLogOut = false);
	        void CustomLog(const std::string logText, const std::string CustomReferenceText = ">", bool noGroupIndentLogOut = false);
	        // ifdef
	        void assert(bool condition, const std::string &message = "true");
	        // Count
	        void count(const std::string tag = "default");
	        void countReset(const std::string tag = "default");
	        // Windows
	        void message(const std::string &messageText, const std::string &messageTitle, const UINT uType = MB_OK);
	        void clear();
	        // Indent
	        void addIndent(int IndentNumber = -1);
	        void setIndent(int IndentNumber = 4);
	        // DebugSwitchValue_STAR
	        void openDebug();
	        void closeDebug();
	        void SwitchDebug();
	        bool inDebug();
	        // DebugTellSayState_STAR
	        void openDebugTellSayState_STAR();
	        void closeDebugTellSayState_STAR();
	        void SwitchDebugTellSayState_STAR();
	        bool inDebugTellSayState_STAR();
	        // Group
	        void group(const std::string groupName);
	        void groupEnd();
	        std::string getGroupName();
	        // Time
	        void time(const std::string timeTag = "default");
	        void timeEnd(const std::string timeTag = "default");
    };

    std::string console::getTimeText()
    {
    	/*
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::ostringstream timeStream;
        timeStream << std::put_time(&now_tm, "%m/%d/%y %H:%M:%S");
        return timeStream.str();
        */
    }
    std::string console::getLogOutText(const std::string &LogReferenceText, const std::string &logOutText)
    {
        return "[" + getTimeText() + "][" + LogReferenceText + "]: " + logOutText;
    }
    void console::winLog(std::string logText, std::vector<std::string> &returnList)
    {
        if (logText.empty())
        {
            console::winError("console.winLog", "Null Pointer reference", 0x02542);
            return;
        }
        std::string lineText = "";
        for (int i = 0; i < logText.size(); ++i)
        {
            if (logText[i] == '\n')
            {
                returnList.push_back(lineText);
                lineText = "";
            }
            else
            {
                lineText += logText[i];
            }
        }
        if (!lineText.empty())
        {
            returnList.push_back(lineText);
        }
    }
    void console::debugTellSayState()
    {
        if (console::debugTellSayState_STAR)
        {
            if (console::debugSwitchValue_STAR)
                console::CustomLog("Debug mode is started.(--Console)");
            else
                console::CustomLog("Debug mode is turned off.(--Console)");
        }
    }
    std::string console::groupIndentLogOut(const bool noGroupIndentLogOut, const int add)
    {
        if (noGroupIndentLogOut)
            return "";
        std::string line = "";
        int addNumber = groupIndentStack.size() + add;
        for (int i = 0; i < IndentBlockNumber * addNumber; ++i)
            line += " ";
        return line;
    }

    // ConsoleColor
    void console::SetConsoleColor(WORD color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
    void console::ResetConsoleColor()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    // Keyboard
    void console::waitKey(const char downKey)
    {
        while (1)
        {
            char key = getch();
            if (key == downKey)
            {
                return;
            }
        }
    }
    bool console::ifDownKey(const char downKey)
    {
        char key = getch();
        if (key == downKey)
        {
            return true;
        }
        return false;
    }
    // Log
    void console::winError(std::string SourceError, std::string ErrorDescription, int ErrorCode)
    {
        if (SourceError.empty())
        {
            SourceError = "???NULL???";
        }
        if (ErrorDescription.empty())
        {
            ErrorDescription = "???NULL???";
        }
        SetConsoleColor(FOREGROUND_RED);
        std::cout << "Error: an error has occurred in the \""
                  << SourceError
                  << "\" module, \nwhich manually refers to it to ensure information. \nFIX IT based on the information in the module.\nMessage: "
                  << ErrorDescription << "\nError Code: "
                  << ErrorCode << std::endl
                  << "Please fix it according to the content" << std::endl;
        ResetConsoleColor();
    }
    void console::log(std::string logText)
    {
        std::vector<std::string> logTextList;
        winLog(logText, logTextList);
        for (int i = 0; i < logTextList.size(); ++i)
        {
            std::cout << groupIndentLogOut() << getLogOutText("Log", logTextList[i]) << std::endl;
        }
    }
    void console::warn(std::string logText)
    {
        SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN);
        std::vector<std::string> logTextList;
        winLog(logText, logTextList);
        for (int i = 0; i < logTextList.size(); ++i)
        {
            std::cout << groupIndentLogOut() << getLogOutText("Warn", logTextList[i]) << std::endl;
        }
        ResetConsoleColor();
    }
    void console::error(std::string logText)
    {
        std::vector<std::string> logTextList;
        winLog(logText, logTextList);
        SetConsoleColor(FOREGROUND_RED);
        for (int i = 0; i < logTextList.size(); ++i)
        {
            std::cout << groupIndentLogOut() << getLogOutText("Error", logTextList[i]) << std::endl;
        }
        ResetConsoleColor();
    }
    void console::info(std::string logText)
    {
        std::vector <std::string> logTextList;
        winLog(logText, logTextList);
        SetConsoleColor(FOREGROUND_BLUE);
        for (int i = 0; i < logTextList.size(); ++i)
        {
            std::cout << groupIndentLogOut() << getLogOutText("Info", logTextList[i]) << std::endl;
        }
        ResetConsoleColor();
    }
    void console::debug(std::string logText, bool thisStartDebug)
    {
        std::vector <std::string> logTextList;
        winLog(logText, logTextList);
        SetConsoleColor(FOREGROUND_RED | FOREGROUND_BLUE);
        for (int i = 0; i < logTextList.size(); ++i)
        {
            if (debugSwitchValue_STAR || thisStartDebug)
            {
                std::cout << groupIndentLogOut() << getLogOutText("Debug", logTextList[i]) << std::endl;
            }
        }
        ResetConsoleColor();
    }
    void console::staticLog(const std::string logText, bool noGroupIndentLogOut)
    {
        std::cout << groupIndentLogOut(noGroupIndentLogOut) << logText << std::endl;
    }
    void console::CustomLog(const std::string logText, const std::string CustomReferenceText, bool noGroupIndentLogOut)
    {
        std::cout << groupIndentLogOut(noGroupIndentLogOut) << CustomReferenceText << ' ' << logText << std::endl;
    }
    // ifdef
    void console::assert(bool condition, const std::string &message)
    {
        if (!condition)
        {
            console instance;
            instance.CustomLog("Assertion failed: " + message);
        }
    }
    // Count
    void console::count(const std::string tag)
    {
        consoleCountMap[tag]++;
        staticLog(tag + ": " + cnt::to_string(consoleCountMap[tag]));
    }
    void console::countReset(const std::string tag)
    {
        consoleCountMap[tag] = 0;
        staticLog(tag + ": " + cnt::to_string(consoleCountMap[tag]));
    }
    // Windows
    void console::message(const std::string &messageText, const std::string &messageTitle, const UINT uType)
    {
        std::wstring messageTextW = std::wstring(messageText.begin(), messageText.end());
        std::wstring messageTitleW = std::wstring(messageTitle.begin(), messageTitle.end());
        MessageBoxW(NULL, messageTextW.c_str(), messageTitleW.c_str(), uType);
    }
    void console::clear()
    {
        system("cls");
    }
    // Indent
    void console::addIndent(int IndentNumber)
    {
        if (IndentNumber < 0)
        {
            IndentNumber = IndentBlockNumber;
        }
        for (int i = 0; i < IndentNumber; ++i)
        {
            std::cout << ' ';
        }
    }
    void console::setIndent(int IndentNumber)
    {
        if (IndentNumber < 0)
        {
            winError("console.Indent", "The number of indentation cells should not be less than 0", 0x45021);
        }
        else
        {
            IndentBlockNumber = IndentNumber;
            CustomLog("Operation Complete -> Indent.Set");
        }
    }
    // DebugSwitchValue_STAR
    void console::openDebug()
    {
        debugSwitchValue_STAR = true;
        debugTellSayState();
    }
    void console::closeDebug()
    {
        debugSwitchValue_STAR = false;
        debugTellSayState();
    }
    void console::SwitchDebug()
    {
        debugSwitchValue_STAR = !debugSwitchValue_STAR;
        debugTellSayState();
    }
    bool console::inDebug()
    {
        return debugSwitchValue_STAR;
    }
    // DebugTellSayState_STAR
    void console::openDebugTellSayState_STAR()
    {
        debugTellSayState_STAR = true;
    }
    void console::closeDebugTellSayState_STAR()
    {
        debugTellSayState_STAR = false;
    }
    void console::SwitchDebugTellSayState_STAR()
    {
        debugTellSayState_STAR = !debugTellSayState_STAR;
    }
    bool console::inDebugTellSayState_STAR()
    {
        return debugTellSayState_STAR;
    }
    // Group
    void console::group(const std::string groupName)
    {
        std::cout << groupIndentLogOut() << "Group[" << groupIndentStack.size() + 1 << "]: " << groupName << std::endl;
        groupIndentStack.push_back(groupName);
    }
    void console::groupEnd()
    {
        std::cout << groupIndentLogOut(false, -1) << "<- Group-End[" << groupIndentStack.size() << "]: " << groupIndentStack.back() << std::endl;
        groupIndentStack.pop_back();
    }
    std::string console::getGroupName()
    {
        return groupIndentStack.back();
    }
    // Time
    void console::time(const std::string timeTag)
    {
        CustomLog("Add timing to the timing queue: " + timeTag);
        timeNameList.push_back(timeTag);
    }
    void console::timeEnd(const std::string timeTag)
    {
        CustomLog("Removes the timing to the timing queue: " + timeTag);
        // timeNameList.erase(std::remove(timeNameList.begin(), timeNameList.end(), timeTag), timeNameList.end());
    }
};
