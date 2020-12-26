`timescale 1 ns / 1 ps 
module EXReg (
    input CLK,
    input [31:0] PCi,
    input [31:0] opAi,
    input [31:0] opBi,
    input [31:0] extImmi,
    input [31:0] instructioni,

    input [1:0] regDsti,
    input [1:0] aluSrcAi,
    input [2:0] aluSrcBi,
    input [1:0] whatToRegi,
    input regWritei,
    input memWritei,
    input [3:0] ALUOpi,
    input [1:0] readModei,
    input [1:0] movei,

    input branchEXi,

    output [31:0] PCo,
    output [31:0] opAo,
    output [31:0] opBo,
    output [31:0] extImmo,
    output [31:0] instructiono,

    output [1:0] regDsto,
    output [1:0] aluSrcAo,
    output [2:0] aluSrcBo,
    output [1:0] whatToRego,
    output regWriteo,
    output memWriteo,
    output [3:0] ALUOpo,
    output [1:0] readModeo,
    output [1:0] moveo,

    output branchEXo
);

reg [31:0] PCReg;
reg [31:0] opAReg;
reg [31:0] opBReg;
reg [31:0] extImmReg;
reg [31:0] insReg;

reg [1:0] regDstReg;
reg [1:0] aluSrcAReg;
reg [2:0] aluSrcBReg;
reg [1:0] whatToRegReg;
reg regWriteReg;
reg memWriteReg;
reg [3:0] ALUOpReg;
reg [1:0] readModeReg;
reg [1:0] moveReg;

reg branchEXReg;

always @ ( posedge CLK ) begin
    PCReg <= PCi;
    opAReg <= opAi;
    opBReg <= opBi;
    extImmReg <= extImmi;
    insReg <= instructioni;
    regDstReg <= regDsti;
    aluSrcAReg <= aluSrcAi;
    aluSrcBReg <= aluSrcBi;
    whatToRegReg <= whatToRegi;
    regWriteReg <= regWritei;
    memWriteReg <= memWritei;
    ALUOpReg <= ALUOpi;
    readModeReg <= readModei;
    moveReg <= movei;
    branchEXReg <= branchEXi;
end

assign PCo = PCReg;
assign opAo = opAReg;
assign opBo = opBReg;
assign extImmo = extImmReg;
assign instructiono = insReg;

assign regDsto = regDstReg;
assign aluSrcAo = aluSrcAReg;
assign aluSrcBo = aluSrcBReg;
assign whatToRego = whatToRegReg;
assign regWriteo = regWriteReg;
assign memWriteo = memWriteReg;
assign ALUOpo = ALUOpReg;
assign readModeo = readModeReg;
assign moveo = moveReg;

assign branchEXo = branchEXReg;

endmodule