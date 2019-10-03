cmake_minimum_required(VERSION 3.14)
project(Photomosaic)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cs225)
include_directories(cs225/catch)
include_directories(cs225/ColorSpace)
include_directories(cs225/lodepng)
include_directories(tests)
include_directories(util)

add_executable(Photomosaic
        cs225/catch/catch.hpp
        cs225/catch/catchmain.cpp
        cs225/ColorSpace/ColorSpace.cpp
        cs225/ColorSpace/ColorSpace.h
        cs225/ColorSpace/Comparison.cpp
        cs225/ColorSpace/Comparison.h
        cs225/ColorSpace/Conversion.cpp
        cs225/ColorSpace/Conversion.h
        cs225/ColorSpace/Utils.h
        cs225/lodepng/lodepng.cpp
        cs225/lodepng/lodepng.h
        cs225/HSLAPixel.cpp
        cs225/HSLAPixel.h
        cs225/LUVAPixel.cpp
        cs225/LUVAPixel.h
        cs225/PNG.cpp
        cs225/PNG.h
        cs225/RGB_HSL.h
        cs225/RGB_LUV.h
        tests/tests_part1.cpp
        tests/tests_part1.h
        tests/tests_part2.cpp
        util/coloredout.cpp
        util/coloredout.h
        util/no_sort.h
        util/util.cpp
        util/util.h
        kdtree.h
        kdtree.hpp
        kdtree_extras.hpp
        main.cpp
        maptiles.cpp
        maptiles.h
        mosaiccanvas.cpp
        mosaiccanvas.h
        point.cpp
        point.h
        sourceimage.cpp
        sourceimage.h
        tileimage.cpp
        tileimage.h)
