/*
 * Components for fileStorage operations -*- C++ -*-
 *
 * Copyright (C) [Year] [Your Name or Organization]
 *
 * This file is part of the custom fileStorage library for C++.
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

/** @file fileStorage.h
 *  This is a custom C++ Library header for fileStorage operations.
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

namespace cnt{
    class fileStorage{
        private:
            std::string _readFile(const std::string filePath)
            {
                std::ifstream file(filePath);
                if (!file)
                    throw std::runtime_error("Can not open file for reading.");
                std::string line, returnText = "";
                while (std::getline(file, line))
                    returnText += line + "\n";
                file.close();
                return returnText;
            }
            int _writeFile(const std::string filePath, const std::string fileText)
            {
                std::ofstream file(filePath);
                if (!file)
                    throw std::runtime_error("Can not open file for reading.");
                file << fileText;
                file.close();
                return 1;
            }
            std::vector<char> _readBinaryFile(const std::string filePath)
            {
                std::ifstream file(filePath);
                if (!file)
                    throw std::runtime_error("Can not open file for reading.");
                file.seekg(0, file.end);
                std::streampos fileSize = file.tellg();
                file.seekg(0, file.beg);
                std::vector<char> buffer(fileSize);
                file.read(buffer.data(), fileSize);
                if (!file)
                    throw std::runtime_error("An error occurred while reading the file.");
                file.close();
                return buffer;
            }
            int _writeBinaryFile(const std::string filePath, const char data[])
            {
                std::ofstream file(filePath);
                if (!file)
                    throw std::runtime_error("Can not open file for reading.");
                file.write(reinterpret_cast<const char *>(&data), sizeof(data));
                file.close();
                return 1;
            }
        public:
            std::string readFile(const std::string filePath)
            {
                try
                {
                    return _readFile(filePath);
                }
                catch (const std::exception &err)
                {
                    std::string errorMessage = err.what();
                    return "{ERROR: " + errorMessage + "}";
                }
            }
            std::string writeFile(const std::string filePath, const std::string fileText)
            {
                try
                {
                    switch (_writeFile(filePath, fileText))
                    {
                        case 1:
                            return "{SUCCESS}";
                            break;
                        default:
                            return "{ERROR: There may be potential problems with the function}";
                            break;
                    }
                }
                catch (const std::exception &err)
                {
                    std::string errorMessage = err.what();
                    return "{ERROR: " + errorMessage + "}";
                }
            }
            std::vector<char> readBinaryFile(const std::string filePath)
            {
                try
                {
                    return _readBinaryFile(filePath);
                }
                catch (const std::exception &err)
                {
                    std::string errorMessageInformation = err.what();
                    std::string errorMessage = "{ERROR: " + errorMessageInformation + "}";
                    std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                    return errorBuffer;
                }
            }
            std::string writeBinaryFile(const std::string filePath, const char data[])
            {
                try
                {
                    switch (_writeFile(filePath, data))
                    {
                    case 1:
                        return "{SUCCESS}";
                        break;
                    default:
                        return "{ERROR: There may be potential problems with the function}";
                        break;
                    }
                }
                catch (const std::exception &err)
                {
                    std::string errorMessage = err.what();
                    return "{ERROR: " + errorMessage + "}";
                }
            }
    };
}