`timescale 1 ns / 1 ps
`define cycleTime 10
module ALUSim ();

reg [31:0] in1;
reg [31:0] in2;
reg [3:0] ALUCtr;

wire [31:0] ALURes;
wire [31:0] lo;
wire        zero;

ALU ALU (in1,in2,ALUCtr,ALURes,lo,zero);
initial 
begin
    in1 <= 32'b0110; in2 <= 32'b0110;
    ALUCtr <= 4'b0110; #20;
    in1 <= 32'b0110; in2 <= 32'b1011; 
    ALUCtr <= 4'b0010; #20;
    ALUCtr <= 4'b0110; #20;
    ALUCtr <= 4'b0000; #20;
    ALUCtr <= 4'b0001; #20;
    ALUCtr <= 4'b0100; #20;
    ALUCtr <= 4'b0111; #20;
    ALUCtr <= 4'b0101; #20;
    ALUCtr <= 4'b1000; #20;
    ALUCtr <= 4'b1001; #20;
    ALUCtr <= 4'b1010; #20;
    ALUCtr <= 4'b1011; #20;
    ALUCtr <= 4'b1100; #20;
end


endmodule