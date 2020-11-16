module ALU (
    input [31:0] in1,
    input [31:0] in2,
    input [3:0] ALUCtr,

    output reg [31:0] ALURes,
    output reg [31:0] lo,
    output reg        zero    
);

reg [63:0] tmpRes;
always @ ( * )
begin
    case (ALUCtr)
        4'b0010: ALURes = in1 + in2;
        4'b0110: 
        begin 
            ALURes = in1 - in2;
            if(in1 == in2) zero = 1;
            else           zero = 0;
            // can not use ALURes == 0 as condition
            // because two statements run at the same time
        end
        4'b0000: ALURes = in1 & in2;
        4'b0001: ALURes = in1 | in2;
        4'b0100: ALURes = (~in1 & in2) | (in1 & ~in2);
        4'b0111: ALURes = ($signed(in1) < $signed(in2)) ? 1 : 0;
        4'b1111: ALURes = (in1 < in2) ? 1 : 0;
        4'b0101: ALURes = ~(in1 | in2);
        4'b1000: ALURes = in1 << in2;
        4'b1001: ALURes = in1 >> in2;
        4'b1010: ALURes = $signed(in1) >>> in2;
        4'b1011: {ALURes,lo} = $signed(in1) * $signed(in2);
        4'b1100:
        begin
            ALURes = $signed(in1) % $signed(in2);
            lo     = $signed(in1) / $signed(in2);
        end
    endcase
end

endmodule 