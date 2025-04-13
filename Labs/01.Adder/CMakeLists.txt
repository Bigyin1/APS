
verilate(
    ${V_EXE_NAME}
    PREFIX ${V_EXE_NAME}
    SOURCES board_files/nexys_adder.sv fulladder32.sv cla4.sv
    VERILATOR_ARGS -Wno-fatal --cc --exe
)


SET(TB_NAME Vtb)
add_executable(${TB_NAME} tb.cpp)

if ($<TARGET_NAME:target> STREQUAL ${TB_NAME})
    verilate(
        ${TB_NAME}
        PREFIX ${TB_NAME}
        SOURCES tb_fulladder32.sv fulladder32.sv cla4.sv
        VERILATOR_ARGS -Wno-fatal --timing --cc --exe
    )
endif()
