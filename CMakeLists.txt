cmake_minimum_required(VERSION 3.20)
project(win32api)


# CppCommon
add_library(CppCommon INTERFACE)
target_compile_features(CppCommon INTERFACE cxx_std_17)
target_compile_options(CppCommon INTERFACE
    /utf-8 /W4 /WX /EHa- /GR-
)
target_compile_definitions(CppCommon INTERFACE
    _HAS_EXCEPTIONS=0
    UNICODE
    _UNICODE
)


#
add_subdirectory(messagebox)
