


module fulladder32 (
    input  logic [31:0] a_i,
    input  logic [31:0] b_i,
    input  logic        carry_i,
    output logic [31:0] sum_o,
    output logic        carry_o
);

  localparam cla_count = 8;

  logic [cla_count:0] carry;

  assign carry[0] = carry_i;

  assign carry_o  = carry[cla_count];

  genvar i;

  for (i = 0; i < cla_count; ++i) begin : gen_ripple_chain
    cla4 u_cla4 (
        .a_i    (a_i[i*4+3 : i*4]),
        .b_i    (b_i[i*4+3 : i*4]),
        .carry_i(carry[i]),
        .sum_o  (sum_o[i*4+3 : i*4]),
        .carry_o(carry[i+1])
    );
  end


endmodule
