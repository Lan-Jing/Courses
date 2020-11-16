`timescale 1 ns / 1 ps 
module regMux (
    input [4:0]  rt,
    input [4:0]  rd,
    
    input [1:0] regDst,

    output [4:0] WAddr
);

reg [4:0] WAddr;

always @ ( * ) begin
    case (regDst)
        2'b00: WAddr = rd;
        2'b01: WAddr = rt;
        2'b10: WAddr = 31;
    endcase    
end

endmodule