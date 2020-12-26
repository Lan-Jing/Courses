`timescale 1 ns / 1 ps 
module MEMReg (
    input CLK,
    input [31:0] PCPlus4i,
    input [31:0] ALUResi,
    input [31:0] Wloi, 

    input [4:0] regWAddri,
    input [1:0] whatToRegi,
    input regWritei,
    input memWritei,
    input [31:0] memDatai,
    input [1:0] readModei,
    input [1:0] movei,

    output [31:0] PCPlus4o,
    output [31:0] ALUReso,
    output [31:0] Wloo,

    output [4:0] regWAddro,
    output [1:0] whatToRego,
    output regWriteo,
    output memWriteo,
    output [31:0] memDatao,
    output [1:0] readModeo,
    output [1:0] moveo
);

reg [31:0] PCPlus4Reg;
reg [31:0] ALUResReg;
reg [31:0] WloReg;
reg [4:0] regWAddrReg;
reg [1:0] whatToRegReg;
reg regWriteReg;
reg memWriteReg;
reg [31:0] memDataReg;
reg [1:0] readModeReg;
reg [1:0] moveReg;

always @ (posedge CLK) begin
    PCPlus4Reg <= PCPlus4i;
    ALUResReg <= ALUResi;
    WloReg <= Wloi;
    regWAddrReg <= regWAddri;
    whatToRegReg <= whatToRegi;
    regWriteReg <= regWritei;
    memWriteReg <= memWritei;
    memDataReg <= memDatai;
    readModeReg <= readModei;
    moveReg <= movei;
end

assign PCPlus4o = PCPlus4Reg;
assign ALUReso = ALUResReg;
assign Wloo = WloReg;
assign regWAddro = regWAddrReg;
assign whatToRego = whatToRegReg;
assign regWriteo = regWriteReg;
assign memWriteo = memWriteReg;
assign memDatao = memDataReg;
assign readModeo = readModeReg;
assign moveo = moveReg;

endmodule 