cmake_minimum_required(VERSION 3.15)

project(lv2tutorial LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_subdirectory(LV2Stub)
add_subdirectory(myAmp)
add_subdirectory(myAmp_GTK)
add_subdirectory(myAmp_GTK3)
add_subdirectory(myAmp_Xputty)
add_subdirectory(mySimplePolySynth)
add_subdirectory(mySimpleSynth)
add_subdirectory(mySimpleSynth_1)
add_subdirectory(mySineSynth)
add_subdirectory(myTestTone)

include(Utils.cmake)
#print_all_variables()
