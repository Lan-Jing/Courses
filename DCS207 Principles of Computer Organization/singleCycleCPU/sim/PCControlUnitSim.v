`timescale 1 ns / 1 ps
`define cycleTime 10
module PCControlUnitSim();

reg CLK;
always # (`cycleTime / 2) begin
    CLK <= ~CLK;
end

reg [31:0] jrPC;
reg [31:0] resetPC;
reg [2:0]  PCSel;
reg bne;
reg zero;

reg [31:0] ins; 
reg [31:0] imm;   

wire [31:0] PC;
wire [31:0] PCPlus4;
wire [31:0] branchPC;
wire [31:0] jmpPC;

PCCalUnit PCcal (ins,imm,PC,PCPlus4,branchPC,jmpPC);
PCControlUnit PCControl (PCPlus4,branchPC,jmpPC,jrPC,resetPC,PCSel,bne,zero,CLK,PC);

initial begin
    CLK <= 0;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b000; #10;
    PCSel <= 3'b001; bne <= 0; zero <= 0; imm <= 10; #10;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b001; bne <= 0; zero <= 1; imm <= 10; #10;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b001; bne <= 1; zero <= 0; imm <= 10; #10;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b001; bne <= 1; zero <= 1; imm <= 10; #10;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b010; ins <= 32'h08100014; #10;
    PCSel <= 3'b100; resetPC <= 0; #10;
    PCSel <= 3'b011; jrPC <= 12; #10;
end

endmodule