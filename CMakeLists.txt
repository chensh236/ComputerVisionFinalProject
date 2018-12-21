cmake_minimum_required(VERSION 3.12)
project(segment)

set(CMAKE_CXX_STANDARD 14)


if (APPLE)
    set (CMAKE_CXX_FLAGS "-lX11 -L/usr/X11/lib -I/usr/X11/include")
    include_directories(segment)

    add_executable(segment
            segment/public.h
           segment/main.cpp
           segment/segment.cpp
           segment/segment.h
           segment/CImg.h
            segment/Hough.cpp
            segment/Hough.h
            segment/warp.cpp
            segment/warp.h
            segment/Projection.cpp
            segment/Projection.h
            segment/cutPiece.cpp
            segment/cutPiece.h
            segment/equalization.cpp
            segment/equalization.h
            segment/reshape.cpp segment/reshape.h
            segment/canny.cpp segment/canny.h)
endif (APPLE)

if(UNIX AND NOT APPLE)
    include_directories(segment)
    add_executable(segment
            segment/public.h
           segment/main.cpp
           segment/segment.cpp
           segment/segment.h
           segment/CImg.h
            segment/Hough.cpp
            segment/Hough.h
            segment/warp.h
            segment/warp.cpp
            segment/Projection.cpp
            segment/Projection.h
            segment/cutPiece.cpp
            segment/cutPiece.h
            segment/equalization.cpp
            segment/equalization.h
            segment/reshape.cpp segment/reshape.h
            segment/canny.cpp segment/canny.h)
    set (CMAKE_CXX_FLAGS "-lX11 -pthread")
    message(STATUS "Looking for X11...")
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    target_link_libraries(a4detect-linux ${X11_LIBRARIES})
endif(UNIX AND NOT APPLE)

if (WIN32)
    set (CMAKE_CXX_FLAGS)
    set (CMAKE_SHARED_LINKER_FLAGS "-lgdi32")


    include_directories(segment)
    add_executable(segment
            segment/public.h
           segment/main.cpp
           segment/segment.cpp
           segment/segment.h
           segment/CImg.h
            segment/Hough.cpp
            segment/Hough.h
            segment/warp.h
            segment/warp.cpp
            segment/Projection.cpp
            segment/Projection.h
            segment/public.h
            segment/cutPiece.cpp
            segment/cutPiece.h
            segment/cutPiece.h segment/prepareLearning.cpp segment/prepareLearning.h
            segment/equalization.cpp
            segment/equalization.h
            segment/reshape.cpp segment/reshape.h
            segment/canny.cpp segment/canny.h)
endif(WIN32)
include_directories(.)