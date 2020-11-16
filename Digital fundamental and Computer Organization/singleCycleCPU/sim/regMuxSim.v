`timescale 1 ns / 1 ps
`define cycleTime 10
module regMuxSim ();

reg [4:0] rt;
reg [4:0] rd;
reg [1:0] regDst;

wire [4:0] WAddr;

regMux rMux (rt,rd,regDst,WAddr);

initial begin
    rt <= 10; rd <= 20;
    regDst <= 2'b00; #10;
    regDst <= 2'b01; #10;
    regDst <= 2'b10; #10;
end

endmodule