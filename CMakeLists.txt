cmake_minimum_required(VERSION 3.17)
project(APS LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)


FetchContent_Declare(
    IXWebSocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
)


FetchContent_MakeAvailable(IXWebSocket)


find_package(verilator HINTS $ENV{VERILATOR_ROOT})
add_subdirectory(Labs/01.Adder)
