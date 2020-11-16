`timescale 1 ns / 1 ps 
module IFReg (
    input CLK,
    input [31:0] PCi,
    input [31:0] instructioni,
    input reset,

    output [31:0] PCo,
    output [31:0] instructiono,

    output [1:0] regDst,
    output [1:0] aluSrcA,
    output [2:0] aluSrcB,
    output [1:0] whatToReg,
    output regWrite,
    output memWrite,
    output [2:0] PCSel,
    output bne,
    output [3:0] ALUOp,
    output immExpand,
    output [1:0] readMode,
    output [1:0] move
);

reg [31:0] PCReg;
reg [31:0] insReg;

always @ ( posedge CLK) begin
    PCReg <= PCi;
    insReg <= instructioni;
end

assign PCo = PCReg;
assign instructiono = insReg;

controlUnit controlUnit (
    insReg[31:26],
    insReg[5:0],
    reset,

    regDst,
    aluSrcA,
    aluSrcB,
    whatToReg,
    regWrite,
    memWrite,
    PCSel,
    bne,
    ALUOp,
    immExpand,
    readMode,
    move
);

endmodule