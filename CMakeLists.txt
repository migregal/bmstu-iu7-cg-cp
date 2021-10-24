cmake_minimum_required(VERSION 3.20)
project(bmstu_iu7_cg_course)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(QT_GUI
        ui/qt/design.ui
        ui/qt/mainwindow.h
        ui/qt/mainwindow.cpp
        )

set(ENGINE
        src/engine/Engine.h
        src/engine/Engine.cpp

        src/engine/qt/QtEngine.h
        src/engine/qt/QtEngine.cpp
        )

set(DRAWER
        src/drawer/Drawer.h
        
        src/drawer/qt/QtDrawer.h
        src/drawer/qt/QtDrawer.cpp
        )      

set(FRACTALS
        src/fractals/Fractal.h
        
        src/fractals/mandelbulb/Mandelbulb.h
        src/fractals/mandelbulb/Mandelbulb.cpp

        # src/fractals/julia/Julia.h
        # src/fractals/julia/Julia.cpp
        )      

set(MATH 
        src/math/Matrix.h
        src/math/Vector.h
        )

add_executable(${PROJECT_NAME}
        ${QT_GUI}
        ${ENGINE}
        ${DRAWER}
        ${FRACTALS}
        ${MATH}
        main.cpp
        )
        
target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        )

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
        ./src/
        ./ui/
        )
