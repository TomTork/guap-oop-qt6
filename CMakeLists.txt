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
        mainwindow.cpp
        mainwindow.h
        repair/repairfactory.h
        command/commandmanager.h
        command/repaircommand.h
        employees/employee.h
        command/changeratecommand.h
        repair/decoratedrepair.h
        employees/employeewindow.cpp
        employees/employeewindow.h
        clients/clientwindow.cpp
        clients/clientwindow.h
        clients/client.h
        command/changeclientcommand.h
)
target_link_libraries(untitled1
  Qt::Core
  Qt::Gui
  Qt::Widgets
)
