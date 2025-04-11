
verilate(Vnexys
    SOURCES alu_opcodes_pkg.sv board_files/nexys_alu.sv alu.sv 
    TOP_MODULE nexys_alu

    VERILATOR_ARGS -Wno-fatal --cc --exe
    )


    