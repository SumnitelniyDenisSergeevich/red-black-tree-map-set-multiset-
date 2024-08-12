cmake_minimum_required(VERSION 3.14)

project(RB LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GTEST_INCLUDE_DIR "D:/CommonProgrammLibs/googletest/googletest/include/")
set(GTEST_MAIN_LIBRARY "D:/CommonProgrammLibs/build-googletest-Desktop_Qt_5_12_12_MinGW_64_bit-Release/lib/libgtest_main.a")
set(GTEST_LIBRARY "D:/CommonProgrammLibs/build-googletest-Desktop_Qt_5_12_12_MinGW_64_bit-Release/lib/libgtest.a")

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
#find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_executable(RB
    s21_mapTests.h
    s21_setTests.h
    s21_vectorTests.h
    s21_multisetTests.h
	s21_arrayTests.h
    test_s21_containers.cpp
    RBTree.h
	s21_array.h
    s21_set.h
    s21_multiset.h
    s21_pair.h
    s21_map.h
    s21_vector.h
)

target_include_directories(RB PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(RB PRIVATE #[[Qt${QT_VERSION_MAJOR}::Core]] GTest::GTest Threads::Threads)
