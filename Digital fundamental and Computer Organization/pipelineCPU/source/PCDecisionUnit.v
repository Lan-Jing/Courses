`timescale 1 ns / 1 ps
module PCDecisionUnit (
    input [31:0] branchPC,
    input [31:0] jmpPC,
    input [31:0] jrPC,
    input [31:0] resetPC,
    input [2:0] PCSel,
    input bne,
    input [31:0] in1,
    input [31:0] in2,

    output [31:0] branchPCo,
    output branch
);

reg [31:0] branchPCo;
reg branch;

always @ ( * ) begin
    case ( PCSel )
        3'b000: begin
            branch = 0;
            branchPCo = 32'bx;
        end
        3'b001: begin
            branch = ((in1 - in2 == 32'b0) ^ bne );
            branchPCo = branchPC;
        end
        3'b010: begin
            branch = 1;
            branchPCo = jmpPC;
        end
        3'b011: begin
            branch = 1;
            branchPCo = jrPC;
        end
        3'b100: begin
            branch = 1;
            branchPCo = resetPC;
        end
    endcase
end

endmodule