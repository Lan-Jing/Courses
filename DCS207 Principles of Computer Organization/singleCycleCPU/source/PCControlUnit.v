module PCControlUnit (
    input [31:0] PCPlus4,
    input [31:0] branchPC,
    input [31:0] jmpPC,
    input [31:0] jrPC,
    input [31:0] resetPC,
    input [2:0]  PCSel,
    input bne,
    input zero,

    input CLK,
    
    output [31:0] PC
);

reg [31:0] PC;

always @ (posedge CLK) begin
    case (PCSel)
        3'b000: PC <= PCPlus4;
        3'b001: begin
            if((bne == 0 && zero == 1) || (bne == 1 && zero == 0))
                PC <= branchPC;
            else PC <= PCPlus4;
        end
        3'b010: PC <= jmpPC;
        3'b011: PC <= jrPC;
        3'b100: PC <= resetPC; 
    endcase
end

endmodule