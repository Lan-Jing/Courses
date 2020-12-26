`timescale 1 ns / 1 ps 
module PCControlUnit (
    input CLK,
    input [31:0] PCPlus4,
    input [31:0] branchPC,
    input branch,
    
    output [31:0] newPC
);

reg [31:0] newPC;

always @ (posedge CLK) begin
    newPC <= branch == 0 ? PCPlus4 : branchPC;
end

endmodule
