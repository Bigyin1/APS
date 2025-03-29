


module fulladder32(
    input  logic [31:0] a_i,
    input  logic [31:0] b_i,
    input  logic        carry_i,
    output logic [31:0] sum_o,
    output logic        carry_o
);



  assign sum_o = a_i + b_i;

  assign carry_o = carry_i;


endmodule
