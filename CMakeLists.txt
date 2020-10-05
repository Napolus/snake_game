cmake_minimum_required(VERSION 3.10...3.16)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

#project properties
set(NAME_PROJECT Snake_Game)
project(${NAME_PROJECT}
        VERSION 1.0
        DESCRIPTION ""
        HOMEPAGE_URL ""
        LANGUAGES CXX)

#set c++ standard to use
set(CMAKE_CXX_STANDARD 20)


include_directories(includes)
include_directories(TOOLS)
include_directories(Assets)

add_subdirectory(sources)
