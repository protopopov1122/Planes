#  MIT License
#
#  Copyright (c) 2018 Jevgēnijs Protopopovs
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.

cmake_minimum_required(VERSION 3.1)
project(Planes)

set(wxWidgets_USE_LIBS)
if (WIN32)
	if (CMAKE_BUILD_TYPE EQUAL DEBUG)
		set(wxWidgets_CONFIGURATION  mswud)
	else ()
		set(wxWidgets_CONFIGURATION  mswu)
	endif ()
	find_package(wxWidgets COMPONENTS adv core base png)
else (WIN32)
	set(wxWidgets_USE_UNICODE ON)
	if (CMAKE_BUILD_TYPE EQUAL DEBUG)
		set(wxWidgets_USE_DEBUG ON)
	else ()
		set(wxWidgets_USE_DEBUG OFF)
	endif ()
	find_package(wxWidgets COMPONENTS adv core base)
endif (WIN32)
find_package(SQLite3)

if (wxWidgets_FOUND AND SQLITE3_FOUND)
	include(${wxWidgets_USE_FILE})
	include_directories(${CMAKE_SOURCE_DIR}/headers ${SQLITE3_INCLUDE_DIRS})
	file(GLOB_RECURSE SRCS *.cpp)
	add_executable(planes ${SRCS})
	if(MSVC)
		set_target_properties(calx_ui PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
	endif (MSVC)
	target_link_libraries(planes ${wxWidgets_LIBRARIES} ${SQLITE3_LIBRARIES})
	if (WIN32)
		target_link_libraries(planes UxTheme.lib)
	endif (WIN32)
else()
	message("Libraries not found!")
endif()
