
set(SRCS main.cpp
        Shapes.cpp
        window.cpp
        game.cpp
        snake.cpp
        world.cpp
        gameState.cpp
        pauseState.cpp ../GUI/widget.cpp ../GUI/widget.hpp ../GUI/interactWidget.cpp ../GUI/interactWidget.hpp ../GUI/buttonText.cpp ../GUI/buttonText.hpp
        splashState.cpp )

message("sources " ${SRCS})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2.5 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    message(STATUS "SFML_INCLUDE_DIR: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML_LIBRARIES: ${SFML_LIBRARIES}")
    message(STATUS "SFML_VERSION: ${SFML_VERSION}")
    include_directories(${SFML_INCLUDE_DIR})
endif()

find_package(Boost 1.36.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "BOOST_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
endif()

# Define sources and executable
set(EXECUTABLE_NAME ${NAME_PROJECT})
add_executable(${EXECUTABLE_NAME} ${HEADERS} ${SRCS} )
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})