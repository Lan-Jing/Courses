`timescale 1 ns / 1 ps 
module WBReg (
    input CLK,
    input [31:0] PCPlus4i,
    input [31:0] datai,
    input [31:0] ALUResi,
    input [31:0] Wloi,

    input [4:0] WAddri,
    input [1:0] whatToRegi,
    input regWritei,
    input [1:0] movei,

    output [31:0] PCPlus4o,
    output [31:0] datao,
    output [31:0] ALUReso,
    output [31:0] Wloo,

    output [4:0] WAddro,
    output [1:0] whatToRego,
    output regWriteo,
    output [1:0] moveo
);

reg [31:0] PCPlus4Reg;
reg [31:0] dataReg;
reg [31:0] ALUResReg;
reg [31:0] WloReg;
reg [4:0] WAddrReg;
reg [1:0] whatToRegReg;
reg regWriteReg;
reg [1:0] moveReg;

always @ (posedge CLK) begin
    PCPlus4Reg <= PCPlus4i;
    dataReg <= datai;
    ALUResReg <= ALUResi;
    WloReg <= Wloi;
    WAddrReg <= WAddri;
    whatToRegReg <= whatToRegi;
    regWriteReg <= regWritei;
    moveReg <= movei;
end

assign PCPlus4o = PCPlus4Reg;
assign datao = dataReg;
assign ALUReso = ALUResReg;
assign Wloo = WloReg;
assign WAddro = WAddrReg;
assign whatToRego = whatToRegReg;
assign regWriteo = regWriteReg;
assign moveo = moveReg;

endmodule