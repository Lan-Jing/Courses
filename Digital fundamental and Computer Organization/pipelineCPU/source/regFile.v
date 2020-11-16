`timescale 1 ns / 1 ps 
module regFile (
    input         CLK,
    input         regWrite,
    input [1:0]   read,
    input [1:0]   move,

    input [4:0]   rs,
    input [4:0]   rt,
    input [31:0]  WData,
    input [4:0]   WAddr,
    input [31:0]  Wlo,

    output [31:0] rsData,
    output [31:0] rtData
);

reg [31:0] registers [31:0];
reg [31:0] hi;
reg [31:0] lo; 
// the second range is in decimal, meaning the total amount of registers is 32

reg [31:0] rsData;
reg [31:0] rtData;

always @ ( negedge CLK ) begin
    if(read[0] ^ read[1] == 0) begin
        rsData <= rs == 0 ? 32'b0 : registers[rs];
        rtData <= rt == 0 ? 32'b0 : registers[rt];
    end
    else begin
        rsData <= read[0] == 1 ? hi : lo;
        rtData <= 32'b0;
    end
end

always @ ( posedge CLK ) // only write regFile on positive edges
begin
    #1;
    if(regWrite == 1)
    begin
        if(move == 2'b11) begin
            hi <= WData;
            lo <= Wlo;
        end
        else registers[WAddr] <= WData;
    end
end

integer i;
initial begin
    for(i = 0;i < 32;i = i + 1) begin
        registers[i] <= 32'b0;
    end
    hi <= 0;
    lo <= 0;
end

endmodule