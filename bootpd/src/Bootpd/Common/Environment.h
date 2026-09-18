/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef ENVIRONMENT_ENVIRONMENT_H_
#define ENVIRONMENT_ENVIRONMENT_H_

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <memory>
#include <fstream>
#include <cmath>
#include <functional>
#include <sstream>
#include <iostream>
#include <time.h>
#include <limits.h>
#pragma once

typedef uint16_t _USHORT;
typedef int16_t _SHORT;
typedef unsigned long _ULONG;
typedef unsigned int _UINT;
typedef int _INT32;
typedef long _LONG;
typedef unsigned long long _ULONGLONG;
typedef char _BYTE;
typedef std::string _STRING;
typedef bool _BOOL;
typedef std::thread _THREAD;

#ifdef __GNUC__
#include "environment_linux.h"
#else
#include "environment_windows.h"
#endif
#endif
