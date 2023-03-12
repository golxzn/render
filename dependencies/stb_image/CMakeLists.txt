cmake_minimum_required(VERSION 3.23)

project(stb_image LANGUAGES C CXX)

add_library(stb_image STATIC stb_image.cpp stb_image.h)
add_library(stb::image ALIAS stb_image)

target_include_directories(stb_image SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
