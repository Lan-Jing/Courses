module regFile (
    input         CLK,
    input         regWrite,
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

wire [31:0] rsT1;
wire [31:0] rtT1;

assign rsT1 = ((move[0] == 1'b1) ? hi : lo);
assign rtT1 = 32'b0;
assign rsData = ((move[0] ^ move[1] == 0) ? registers[rs] : rsT1);
assign rtData = ((move[0] ^ move[1] == 0) ? registers[rt] : rtT1);

always @ (posedge CLK) // only write regFile on positive edges
begin
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