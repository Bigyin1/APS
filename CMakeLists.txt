cmake_minimum_required(VERSION 3.17)
project(APS LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)

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

cmake_policy(SET CMP0144 NEW)
find_package(verilator)


set(CURR_LAB_DIR Labs/01.Adder)

add_executable(Vnexys Nexys/main.cpp Nexys/Nexys.cpp ${CURR_LAB_DIR}/vidbo.cpp)

target_include_directories(Vnexys PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Nexys)

target_link_libraries(Vnexys PRIVATE ixwebsocket::ixwebsocket nlohmann_json::nlohmann_json fmt::fmt)


add_subdirectory(${CURR_LAB_DIR})
