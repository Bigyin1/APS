
add_executable(Vnexys_adder Nexys/main.cpp Nexys/Nexys.cpp)

target_link_libraries(Vnexys_adder PRIVATE ixwebsocket::ixwebsocket)

verilate(Vnexys_adder
    SOURCES board_files/nexys_adder.sv fulladder32.sv
    TOP_MODULE nexys_adder

    VERILATOR_ARGS -Wno-fatal --cc --exe -DViDBo=1
    )
