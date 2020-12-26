`timescale 1 ns / 1 ps
`define cycleTime 10
module signExtSim ();


reg [15:0]  num;
reg         mode;
wire [31:0] outData;
initial
begin
    num <= 16'h8000; mode <= 0;
    #20;
    num <= 16'h8000; mode <= 1;
    #20;
    num <= 16'h0001; mode <= 1;
    #20;
    num <= 16'h0001; mode <= 0;
end

signExt ext (num,mode,outData);

endmodule