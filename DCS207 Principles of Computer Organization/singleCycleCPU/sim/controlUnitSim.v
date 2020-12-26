`timescale 1 ns / 1 ps
`define cycleTime 10
module controlUnitSim ();

reg       CLK;
reg [7:0] addr;
reg       reset;
wire [31:0] instruction;

initial
begin
    CLK <= 0;
    addr <= 0;
end

always # (`cycleTime / 2)
begin
    CLK <= ~CLK;
end

always @ (posedge CLK)
begin
    if(addr < 27 * 4) 
        addr <= addr + 4;
    else begin
        addr <= 0;
        reset <= 0;
    end
end

insROM ins (CLK,addr,instruction);
    
wire [1:0] regDst;         // 00:rd, 01:rt, 10:31(for jal)
wire [1:0] aluSrcA;        // 00:rs, 01:rt, 10:shamt, 11:signExt(imm)
wire [2:0] aluSrcB;        // 000:rt, 001:rs, 010:signExt(imm), 011:shamt, 100:16(lui)
wire [1:0] whatToReg;      // 00:from ALU, 01:from MEM, 10 from PC
wire regWrite;             // 1:enable
wire memWrite;             // 1:enable 
wire [2:0] PCSel;          // selectin next PC
wire bne;                  // differentiate beq and bne
wire [3:0] ALUOp;          // for ALUControl 
wire immExpand;            // 1:signExt
wire [1:0] readMode;       // 00: single byte, 01: half-word, 1x: word
wire [1:0] move;           // 00: not move, 01: mfhi, 10: mflo;

controlUnit control(
    instruction[31:26],
    instruction[5:0],
    reset,
    
    regDst,        // 00:rd, 01:rt, 10:31(for jal)
    aluSrcA,       // 00:rs, 01:rt, 10:shamt, 11:signExt(imm)
    aluSrcB,       // 00:rt, 01:rs, 10:signExt(imm), 11:16(lui)
    whatToReg,      // 0:from ALU, 1:from MEM
    regWrite,      // 1:enable
    memWrite,      // 1:enable 
    PCSel,         
    bne,           // differentiate beq and bne
    ALUOp,         // for ALUControl 
    immExpand,     // 1:signExt
    readMode,      // 00: single byte, 01: half-word, 1x: word
    move
);

wire [3:0] ALUCtr;
ALUControlUnit aluCtr (
    ALUOp,
    instruction[5:0],

    ALUCtr
);

endmodule