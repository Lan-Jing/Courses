`timescale 1 ns / 1 ps
`define cycleTime 10
module dataRAMSim();

reg  CLK;
reg  reset;
reg  MEMWrite;
reg  [7:0] addr;
reg  [1:0] readMode;
reg  [31:0] inData;
wire [31:0] outData;

initial 
begin
    CLK <= 0; reset <= 1; #40; reset <= 0;
    MEMWrite <= 1; readMode <= 2'b00; addr <= 8'd0; inData <= 32'hbbbba00a;
    #40;
    MEMWrite <= 1; readMode <= 2'b01; addr <= 8'd2; inData <= 32'hbbbba00a;
    #40;
    MEMWrite <= 1; readMode <= 2'b10; addr <= 8'd4; inData <= 32'hbbbba00a;
    #40;
    MEMWrite <= 0; readMode <= 2'b00; addr <= 8'd0; inData <= 0; 
    #40; 
    MEMWrite <= 0; readMode <= 2'b01; addr <= 8'd0; inData <= 0; 
    #40;
    MEMWrite <= 0; readMode <= 2'b10; addr <= 8'd0; inData <= 0; 
    #40;
    MEMWrite <= 0; readMode <= 2'b10; addr <= 8'd4; inData <= 0;
end

RAM RAM (CLK,MEMWrite,readMode,addr,inData,outData);
always # (`cycleTime / 2)
begin
    CLK <= ~CLK;
end


endmodule