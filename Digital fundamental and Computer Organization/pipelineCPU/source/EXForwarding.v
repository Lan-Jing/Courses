`timescale 1 ns / 1 ps 
module EXForwarding (
    input [1:0] moveWB,
    input [4:0] dstWb,
    input [31:0] ALUResWB,
    input [31:0] WloWB,

    input [1:0] moveMEM,
    input [4:0] dstMEM,
    input [31:0] ALUResMEM,
    input [31:0] WloMEM,

    input [1:0] moveEX,
    input [4:0] rtEX,
    input [4:0] rsEX,
    input [31:0] in1EX,
    input [31:0] in2EX,
    input instructionEXType,

    output [31:0] in1,
    output [31:0] in2
);

reg [31:0] in1;
reg [31:0] in2;

/*
    mind that if the two instructions ahead are both changing 
    the value need to be used now, forward the value from MEM stage 
    first, which is the newest, instead of WB stage.
*/
always @ ( * ) begin
    if(rsEX == dstMEM)          in1 = ALUResMEM;
    else if((moveEX == 2'b01 || moveEX == 2'b10) && (moveMEM == 2'b11 || moveWB == 2'b11))
    begin
        if(moveMEM == 2'b11) begin
            if(moveEX == 2'b01) in1 = ALUResMEM;
            else                in1 = WloMEM;
        end
        else begin
            if(moveEX == 2'b01) in1 = ALUResWB;
            else                in1 = WloWB;
        end
    end
    if(rsEX == dstWb)           in1 = ALUResWB;
    else                        in1 = in1EX;
    
    if(instructionEXType == 1) begin
        if(rtEX == dstMEM)      in2 = ALUResMEM;
        else if(rtEX == dstWb)  in2 = ALUResWB;    
        else                    in2 = in2EX;
    end
    else                        in2 = in2EX;
end

endmodule 