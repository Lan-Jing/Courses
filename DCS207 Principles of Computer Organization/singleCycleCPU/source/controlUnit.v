`timescale 1ns / 1ps
module controlUnit(
    input [5:0] opCode,
    input [5:0] funct,
    input       reset,
    
    output reg [1:0] regDst,    // 00:rd, 01:rt, 10:31(for jal)
    output reg [1:0] aluSrcA,   // 00:rs, 01:rt, 10:signExt(imm)
    output reg [2:0] aluSrcB,   // 000:rt, 001:rs, 010:signExt(imm), 011:shamt, 100:16(for lui)
    output reg [1:0] whatToReg, // 00:from ALU, 01:from MEM, 10:from PC(for jal)
    output reg regWrite,        // 1:enable
    output reg memWrite,        // 1:enable 
    output reg [2:0] PCSel,     // 000:PC+4, 001: branchPC, 010: jmpPC, 011: jrPC, 100: resetPC
    output reg bne,             // differentiate beq and bne
    output reg [3:0] ALUOp,     // for ALUControl 
    output reg immExpand,       // 1:signExt
    output reg [1:0] readMode,  // 00: single byte, 01: half-word, 1x: word
    output reg [1:0] move       // 00: not move, 01: mfhi, 10: mflo, 11: mul or div
);

always @ ( * )
begin
    if(reset == 0) begin
        regWrite = 0;
        memWrite = 0;
        PCSel = 3'b100;
    end
    else
        casex (opCode)
            6'b001111:    //lui
            begin
                regDst = 2'b01;
                aluSrcA = 2'b10; // sign(imm) and 16
                aluSrcB = 3'b100;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b1000;
                immExpand = 0; 
                readMode = 2'bxx; 
                move = 2'b00;  
            end   
            6'b000000:    //R-type
            begin
                regDst = 2'b00;
                whatToReg = 2'b00;
                memWrite = 0;
                bne = 0;
                ALUOp = 4'b0111;
                immExpand = 1'bx; 
                readMode = 2'bxx;

                if(funct == 6'b010000) // mfhi
                    move = 2'b01; 
                else if (funct == 6'b010010) // mflo
                    move = 2'b10; 
                else if (funct == 6'b011000 || funct == 6'b011010)
                    move = 2'b11;    // mult or div
                else 
                    move = 2'b00;    // others

                if(funct == 6'b001000) begin
                    regWrite = 0; PCSel = 3'b011; // jr
                end
                else if(funct == 6'b011000 || funct == 6'b011010 ||
                        funct == 6'b010000 || funct == 6'b010010) begin
                    regWrite = 1; PCSel = 3'b000; 
                    // regWrite is needed in all these instructions
                end // mult, div, mfhi and mflo
                else begin
                    regWrite = 1; PCSel = 3'b000;
                end // others

                if(funct == 6'b000000 || funct == 6'b000010 || funct == 6'b000011) begin
                    aluSrcA = 2'b01; aluSrcB = 3'b011;
                end // sll, srl, sra : rt and shamt
                else if(funct == 6'b000100 || funct == 6'b000110 || funct == 6'b000111) begin
                    aluSrcA = 2'b01; aluSrcB = 3'b001;
                end // sllv, srlv, srav : rt and rs
                else begin
                    aluSrcA = 2'b00; aluSrcB = 3'b000;
                end // others : rs and rt
            end
            6'b00001x:    //J-type
            begin
                aluSrcA = 2'bxx;
                aluSrcB = 3'bxxx;
                memWrite = 0;
                PCSel = 3'b010;
                bne = 0;
                ALUOp = 4'bxxxx;
                immExpand = 1'bx; 
                readMode = 2'bxx;
                move = 2'b00;

                if(opCode == 6'b000011) begin
                    regDst = 2'b10; whatToReg = 2'b10; regWrite = 1;
                end // jal
                else begin
                    regDst = 2'bxx; whatToReg = 2'bxx; regWrite = 0; 
                end // j
            end
            6'b100011:    //lw
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b01;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b11; 
                move = 2'b00;  
            end
            6'b100001:    //lh
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b01;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b01;   
                move = 2'b00;
            end
            6'b100000:    //lb
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b01;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b00;  
                move = 2'b00; 
            end
            6'b101011:    //sw
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 1;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b11;   
                move = 2'b00;
            end
            6'b101001:    //sh
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 1;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b01;   
                move = 2'b00;
            end
            6'b101000:    //sb
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 1;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'b00;   
                move = 2'b00;
            end
            6'b000100:    //beq
            begin
                regDst = 2'bxx;
                aluSrcA = 2'b00;
                aluSrcB = 3'b000;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 0;
                PCSel = 3'b001;
                bne = 0;
                ALUOp = 4'b0001;
                immExpand = 1; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b000101:    //bne
            begin
                regDst = 2'bxx;
                aluSrcA = 2'b00;
                aluSrcB = 3'b000;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 0;
                PCSel = 3'b001;
                bne = 1;
                ALUOp = 4'b0001;
                immExpand = 1; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b00100x:    //addi and addiu
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0000;
                immExpand = 1; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b001100:    //andi
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0010;
                immExpand = 0; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b001101:    //ori
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0011;
                immExpand = 0; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b001110:    //xori
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0100;
                immExpand = 0; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b001010:    //slti
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b0101;
                immExpand = 1; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            6'b001011:    //sltiu
            begin
                regDst = 2'b01;
                aluSrcA = 2'b00;
                aluSrcB = 3'b010;
                whatToReg = 2'b00;
                regWrite = 1;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'b1101;
                immExpand = 1; 
                readMode = 2'bxx;
                move = 2'b00;
            end
            default:    
            begin
                regDst = 2'bxx;
                aluSrcA = 2'bxx;
                aluSrcB = 3'bxxx;
                whatToReg = 2'bxx;
                regWrite = 0;
                memWrite = 0;
                PCSel = 3'b000;
                bne = 0;
                ALUOp = 4'bxxxx;
                immExpand = 1'bx; 
                readMode = 2'bxx;
                move = 2'b00;
            end
        endcase
end

endmodule