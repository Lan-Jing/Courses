`timescale 1 ns / 1 ps
module regDataMuxSim ();

reg [31:0] PC;
reg [31:0] outData;
reg [31:0] ALURes;

reg [1:0] whatToReg;

wire [31:0] WData;

initial begin
    PC <= 32'd20; outData <= 32'd10; ALURes <= 32'd100;
    whatToReg <= 2'b00; #10;
    whatToReg <= 2'b01; #10;
    whatToReg <= 2'b10; #10;
end

regDataMux regDMux (PC,outData,ALURes,whatToReg,WData);

endmodule