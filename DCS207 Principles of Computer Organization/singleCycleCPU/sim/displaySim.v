`timescale 1 ns / 1 ps
`define cycleTime 10
module displaySim();

reg CLK;
always # (`cycleTime / 2)
begin
    CLK <= ~CLK;
end

reg [15:0] data;
wire [3:0] segCode;
wire [6:0] seg;

display dis (CLK,data,segCode,seg);
initial begin
    CLK <= 0; 
    data <= 16'haaaa; #10;
    data <= 16'hbbbb; #10;
end

endmodule