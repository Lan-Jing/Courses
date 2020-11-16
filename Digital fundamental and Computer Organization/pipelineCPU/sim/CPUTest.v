`timescale 1 ns / 1 ps 
`define timePeriod 4
module CPUTest ();

reg CLKFast;
reg CLKSlow;
reg [31:0] CLKCount;
reg reset;
reg [31:0] resetPC;
wire [31:0] ALURes;

initial begin
    CLKCount <= 0;
    CLKFast <= 0;
    CLKSlow <= 0;

    reset <= 0;
    resetPC <= 0;

    #20;

    reset <= 1;
end

always # (`timePeriod / 2) begin
    CLKFast <= ~CLKFast;
    if(CLKCount == 32'd5) begin
        CLKSlow <= ~CLKSlow;
        CLKCount <= 0;
    end
    else CLKCount <= CLKCount + 1;
end

pipelineCPU pipelineCPU (CLKFast,CLKSlow,reset,resetPC,ALURes);

endmodule