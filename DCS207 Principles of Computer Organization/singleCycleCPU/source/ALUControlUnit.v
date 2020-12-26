module ALUControlUnit (
    input [3:0] ALUOp,
    input [5:0] funct,

    output [3:0] ALUCtr
);

reg [3:0] ALUCtr;
always @ ( * )
begin
    casex ({ALUOp,funct})
        //非R型
        10'b0000_xxxxxx: ALUCtr = 4'b0010; // add
        10'b0001_xxxxxx: ALUCtr = 4'b0110; // sub
        10'b0010_xxxxxx: ALUCtr = 4'b0000; // and
        10'b0011_xxxxxx: ALUCtr = 4'b0001; // or
        10'b0100_xxxxxx: ALUCtr = 4'b0100; // xor
        10'b0101_xxxxxx: ALUCtr = 4'b0111; // slti
        10'b1101_xxxxxx: ALUCtr = 4'b1111; // slitu
        10'b0110_001010: ALUCtr = 4'b0010; // mfhi also add
        10'b0110_001100: ALUCtr = 4'b0010; // mflo also add
        10'b1000_xxxxxx: ALUCtr = 4'b1000; // lui also sll

        //R型
        10'b0111_100000: ALUCtr = 4'b0010; // add
        10'b0111_100010: ALUCtr = 4'b0110; // sub
        10'b0111_100011: ALUCtr = 4'b0110; // subu
        10'b0111_100100: ALUCtr = 4'b0000; // and
        10'b0111_100111: ALUCtr = 4'b0101; // nor
        10'b0111_100101: ALUCtr = 4'b0001; // or
        10'b0111_100110: ALUCtr = 4'b0100; // xor
        10'b0111_101010: ALUCtr = 4'b0111; // slt
        10'b0111_101011: ALUCtr = 4'b1111; // sltu
        
        10'b0111_000000: ALUCtr = 4'b1000; // sll
        10'b0111_000010: ALUCtr = 4'b1001; // srl
        10'b0111_000011: ALUCtr = 4'b1010; // sra
        10'b0111_000100: ALUCtr = 4'b1000; // sllv
        10'b0111_000110: ALUCtr = 4'b1001; // srlv
        10'b0111_000111: ALUCtr = 4'b1010; // srav       
        
        10'b0111_011000: ALUCtr = 4'b1011; // mul
        10'b0111_011010: ALUCtr = 4'b1100; // div
        
        default: ALUCtr = 4'b0010;
    endcase
end

endmodule