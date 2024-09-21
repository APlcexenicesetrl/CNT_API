#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include "string.h"

namespace cnt
{
    enum class cntObjectType
    {
        Null,
        String,
        Number,
        Object,
        Array,
        Boolean
    };

    struct cntObject 
    {
        cntObjectType type;
        std::string stringValue;
        double numberValue;
        bool booleanValue;
        std::unordered_map<std::string, cntObject> objectValue;
        std::vector<cntObject> arrayValue;

        std::string serialize() const;
    };

    std::string cntObject::serialize() const
    {
        switch (type)
        {
        case cntObjectType::Null:
            return "null";
        case cntObjectType::String:
            return "\"" + stringValue + "\"";
        case cntObjectType::Number:
            return std::to_string(numberValue);
        case cntObjectType::Boolean:
            return booleanValue ? "true" : "false";
        case cntObjectType::Object:
        {
            std::string result = "{";
            for (auto &pair : objectValue)
            {
                result += "\"" + pair.first + "\":" + pair.second.serialize() + ",";
            }
            if (!objectValue.empty())
            {
                result.pop_back();
            }
            result += "}";
            return result;
        }
        case cntObjectType::Array:
        {
            std::string result = "[";
            for (const auto &item : arrayValue)
            {
                result += item.serialize() + ",";
            }
            if (!arrayValue.empty())
            {
                result.pop_back();
            }
            result += "]";
            return result;
        }
        default:
            return "";
        }
    }
}
