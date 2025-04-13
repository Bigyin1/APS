module cla4 (
    input  logic [3:0] a_i,
    input  logic [3:0] b_i,
    input  logic       carry_i,
    output logic [3:0] sum_o,
    output logic       carry_o
);

  logic [3:0] G;
  logic [3:0] P;

  logic [3:0] G_i_0;
  assign G_i_0[0] = G[0];

  logic [3:0] P_i_0;
  assign P_i_0[0] = P[0];

  logic [3:0] C_i;
  assign C_i[0]  = carry_i;


  assign carry_o = G_i_0[3] | (P_i_0[3] & C_i[0]);


  genvar i;
  generate
    ;
    for (i = 0; i < 4; i++) begin : gen_generate_propagate_summ

      assign G[i] = a_i[i] & b_i[i];

      assign P[i] = a_i[i] | b_i[i];

      assign sum_o[i] = a_i[i] ^ b_i[i] ^ C_i[i];
    end

    for (i = 1; i < 4; i++) begin : gen_prefix_carry

      assign G_i_0[i] = G[i] | (P[i] & G_i_0[i-1]);

      assign P_i_0[i] = P_i_0[i-1] & P[i];

      assign C_i[i]   = G_i_0[i-1] | (P_i_0[i-1] & C_i[0]);
    end

  endgenerate

endmodule
