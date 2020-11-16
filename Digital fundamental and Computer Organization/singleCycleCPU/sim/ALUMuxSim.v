`timescale 1 ns / 1 ps
`define cycleTime 10
module ALUMuxSim ();

reg CLK;
always # (`cycleTime / 2) begin
    CLK <= ~CLK;
end

reg [31:0] rsData;
reg [31:0] rtData;
reg [31:0] extImm;
reg [4:0]  shamt;

reg [1:0] aluSrcA;
reg [2:0] aluSrcB;

wire [31:0] in1;
wire [31:0] in2;

ALUMux ALUMux (
    rsData,
    rtData,
    extImm,
    shamt,

    aluSrcA,
    aluSrcB,
    
    in1,
    in2
);

initial begin
    CLK <= 0;
    rsData <= 32'd10; rtData <= 32'd20; extImm <= 32'd5; shamt <= 5'd2;
    aluSrcA <= 2'b00; aluSrcB <= 3'b000; #10; // rs and rt
    aluSrcA <= 2'b01; aluSrcB <= 3'b001; #10; // rt and rs: sllv
    aluSrcA <= 2'b10; aluSrcB <= 3'b100; #10; // imm and 16: lui
    aluSrcA <= 2'b01; aluSrcB <= 3'b011; #10; // rt and shamt: sll
    aluSrcA <= 2'b00; aluSrcB <= 3'b010; #10; // rs and imm: I-type
end

endmodule