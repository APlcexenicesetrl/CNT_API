/*
 * Components for config operations -*- C++ -*-
 *
 * Copyright (C) [Year] [Your Name or Organization]
 *
 * This file is part of the custom config library for C++.
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

/** @file config.h
 *  This is a custom C++ Library header for config operations.
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
#include "windows.h"

namespace cnt{
    class config{
        private:
            std::vector <std::string> configObj;
        public:
            config(const std::string configInfo){
                
            }
    };
}