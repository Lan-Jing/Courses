`timescale 1 ns / 1 ps
`define cycleTime 10
module CPUSim ();

reg CLKFast;
reg CLKSLowed;
reg reset;
reg [31:0] resetPC;

reg [3:0] CLKCount;
always # (`cycleTime / 2) begin
    CLKFast = ~CLKFast;
    if(CLKCount < 4'd10)
        CLKCount = CLKCount + 1;
    else begin
        CLKCount = 0;
        CLKSLowed = ~CLKSLowed;
    end
end

CPU CPU (CLKFast,CLKSLowed,reset,resetPC);

initial begin
    CLKFast <= 0; CLKSLowed <= 0; CLKCount <= 0;
    reset <= 0; resetPC <= 32'b0;
    #100;
    reset <= 1;
end

endmodule