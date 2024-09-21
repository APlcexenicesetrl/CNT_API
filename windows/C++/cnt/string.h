#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace cnt
{
    int size(const std::string &value)
    {
        return value.size();
    }

    int length(const std::string &value)
    {
        return value.length();
    }

    std::string add(const std::string &add1, const std::string &add2)
    {
        return add1 + add2;
    }

    /*int to_int(const std::string &value)
    {
        try{
            return std::stoi(value);
        }
        catch (const std::invalid_argument &e){
            return 0;
        }
        catch (const std::out_of_range &e){
            return 0;
        }
    }*/

    template <typename T>
    std::string to_string(const T &value)
    {
        std::ostringstream os;
        os << value;
        return os.str();
    }

    bool containsElements(const std::vector<std::string> &elementVector, const std::string &element)
    {
        for (const auto &vecElement : elementVector)
        {
            if (vecElement == element)
            {
                return true;
            }
        }
        return false;
    }

    bool contains(const std::string &valueVector, const std::string &value, bool scattered)
    {
        if (!scattered)
        {
            if (valueVector.find(value) == std::string::npos)
                return false;
            return true;
        }
        else
        {
            for (char ch : value)
                if (valueVector.find(ch) == std::string::npos)
                    return false;
            return true;
        }
    }

    bool isFullSpace(const std::string &value)
    {
        for (int i = 0 ; i < value.size() ; ++i)
            if (value[i] != ' ')
                return false;
        return true;
    }

    std::string inver_string(std::string &value)
    {
        std::string returnStr;
        for (int i = value.size() - 1 ; i >= 0 ; --i)
            returnStr += value[i];
        return returnStr;
    }


    std::string to_string(int &value){
	if (value == 0)
		return "0";
	if (value < 0)
		value += -value * 2;
	
	std::string returnStr;
	for (int tmp ; value > 0;){
    	tmp = value % 10;
    	value = value / 10;
    	//std::cout << tmp << value << std::endl;
    	switch(tmp){
    		case 0:
    			returnStr += "0";
    			break;
    		case 1:
    			returnStr += "1";
    			break;
    		case 2:
    			returnStr += "2";
    			break;
    		case 3:
    			returnStr += "3";
    			break;
    		case 4:
    			returnStr += "4";
    			break;
    		case 5:
    			returnStr += "5";
    			break;
    		case 6:
    			returnStr += "6";
    			break;
    		case 7:
    			returnStr += "7";
    			break;
    		case 8:
    			returnStr += "8";
    			break;
    		case 9:
    			returnStr += "9";
    			break;
    		default:
    			break;
    	}
    }
    
    return inver_string(returnStr);
}
}
