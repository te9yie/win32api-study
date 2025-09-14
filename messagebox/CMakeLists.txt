cmake_minimum_required(VERSION 3.20)
project(StudyMessageBox)

add_executable(${PROJECT_NAME} WIN32 main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE CppCommon)
