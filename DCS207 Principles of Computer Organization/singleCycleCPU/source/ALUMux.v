module ALUMux (
    input [31:0] rsData,
    input [31:0] rtData,
    input [31:0] extImm,
    input [4:0]  shamt,

    input [1:0] aluSrcA,
    input [2:0] aluSrcB,

    output [31:0] in1,
    output [31:0] in2
);

reg [31:0] in1;
reg [31:0] in2;

always @ ( * ) begin
    case (aluSrcA)
        2'b00: in1 = rsData;
        2'b01: in1 = rtData;
        2'b10: in1 = extImm;
    endcase
    case (aluSrcB)
        3'b000: in2 = rtData;
        3'b001: in2 = rsData;
        3'b010: in2 = extImm;
        3'b011: in2 = {27'b0,shamt};
        3'b100: in2 = 32'd16;
    endcase
end

endmodule