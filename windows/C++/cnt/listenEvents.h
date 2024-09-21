#pragma once

#include <iostream>
#include <functional>
#include <map>

#include "systemOs.h"
#include "vectorX.h"
#include "console.h"

namespace cnt{
    enum listenEventsType
    {
        consoleSizeChange
    };

    using EventCallback = std::function<void()>;
    std::map<listenEventsType, EventCallback> eventListeners;

    void triggerEvent(listenEventsType eventType)
    {
        if (eventListeners.find(eventType) != eventListeners.end())
        {
            eventListeners[eventType]();
        }
    }

    void listenEvents(listenEventsType eventType, EventCallback callback)
    {
        eventListeners[eventType] = callback;
        switch (eventType)
        {
            case listenEventsType::consoleSizeChange: 
                cnt::systemOs os;
                if (!(os.fixedConsoleSize.X == os.consoleWindowsSize.X && os.fixedConsoleSize.Y == os.consoleWindowsSize.Y))
                    triggerEvent(eventType);
                break;
            default:
                break;
        }
    }
}