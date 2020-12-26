`timescale 1 ns / 1 ps
`define cycleTime 10
module regFileSim ();

reg CLK;
reg regWrite;
reg [1:0] move;

reg [4:0] rs;
reg [4:0] rt;
reg [31:0] WData;
reg [4:0]  WAddr;
reg [31:0] Wlo;

wire [31:0] rsData;
wire [31:0] rtData;

regFile Reg (CLK,regWrite,move,
             rs,rt,WData,WAddr,Wlo,
             rsData,rtData);
initial 
begin
    CLK <= 0;
    reset <= 1; regWrite <= 1; WData <= 32'haaaa; WAddr <= 5'd10;
    #10;
    reset <= 1; regWrite <= 1; WData <= 32'hbbbb; WAddr <= 5'd20;
    #10;
    reset <= 1; regWrite <= 0; rs <= 5'd10; rt <= 5'd20; move <= 0;
    #10;
    reset <= 0; regWrite <= 0; rs <= 5'd10; rt <= 5'd20; move <= 0;
    #10;
    reset <= 1; regWrite <= 1; WData <= 2; Wlo <= 10; move <= 2'b11;
    #10;
    reset <= 1; regWrite <= 0; move <= 2'b01;
    #10 
    reset <= 1; regWrite <= 0; move <= 2'b10;
end

always # (`cycleTime / 2)
begin
    CLK <= ~CLK;
end

endmodule