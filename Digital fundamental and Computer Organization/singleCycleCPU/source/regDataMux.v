module regDataMux (
    input [31:0] PC,
    input [31:0] outData,
    input [31:0] ALURes,

    input [1:0] whatToReg,

    output [31:0] WData
);

reg [31:0] WData;

always @ ( * ) begin
    case (whatToReg)
        2'b00: WData = ALURes;
        2'b01: WData = outData;
        2'b10: WData = PC + 4;
    endcase
end

endmodule