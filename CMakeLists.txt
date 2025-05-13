

set( CMAKE_CXX_COMPILER "C:/Program Files/Mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/Program Files/Mingw64/bin/gcc.exe" )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_minimum_required(VERSION 3.10)

project(pcynlitx_lib VERSION 1.0)



add_executable(pcynlitx_library_test  

        ${CMAKE_CURRENT_SOURCE_DIR}/pcynlitx_library_test.cpp)


target_link_libraries(pcynlitx_library_test PUBLIC pcynlitx_lib)

add_library(pcynlitx_lib 

    ${CMAKE_CURRENT_SOURCE_DIR}/pcynlitx

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_data_holder.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_data_holder.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_locker.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_locker.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/synchronizer.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/synchronizer.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/synchronizer_channel.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/channel.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/function_blocker.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/function_blocker.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_creator.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_blocker.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/thread_blocker.cpp
)


target_link_directories(pcynlitx_lib PUBLIC 

    ${CMAKE_CURRENT_SOURCE_DIR}
)



target_include_directories(pcynlitx_lib PUBLIC 

    ${CMAKE_CURRENT_SOURCE_DIR}

 )

