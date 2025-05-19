cmake_minimum_required(VERSION 3.30)
project(untitled1)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt6 COMPONENTS
  Core
  Gui
  Widgets
  REQUIRED)

add_executable(untitled1 main.cpp
        estate.h
        states.h
        estate.cpp
        states.cpp
        calculationFacade.h
        calculationfacade.cpp
        apartmentCalc.h
        apartmentCalc.cpp
        luxuriousapartmentCalc.h
        luxuriousapartmentCalc.cpp
        townhouseCalc.h
        cottageCalc.h
        widget.cpp
        widget.h
        ui_widget.h
        economCalc.h
        luxuriousCalc.h
        abstractCalc.h
)
target_link_libraries(untitled1
  Qt::Core
  Qt::Gui
  Qt::Widgets
)

