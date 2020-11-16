module PCCalUnit (
    input [25:0] Jinstruction,  // for computing {PC[31:27],J[25:0],00}
    input [31:0] immediate,     // for computing PC + 4 + imm
    input [31:0] oldPC,         // for computing PC + 4

    output [31:0] PCPlus4,
    output [31:0] branchPC,
    output [31:0] jmpPC
);

assign PCPlus4 = oldPC + 4;
assign branchPC = PCPlus4 + (immediate << 2);
assign jmpPC = {oldPC[31:27],Jinstruction[25:0],2'b00};

endmodule