cmake_minimum_required(VERSION 3.17)
project(APS LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)


FetchContent_Declare(
    IXWebSocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
)
FetchContent_MakeAvailable(IXWebSocket)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)


FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)


find_package(verilator)
add_subdirectory(Labs/01.Adder)
