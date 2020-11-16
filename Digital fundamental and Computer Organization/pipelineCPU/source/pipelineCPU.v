`timescale 1 ns / 1 ps 
module pipelineCPU (
    input CLKFast,
    input CLKSlow,
    input reset,
    input [31:0] resetPC,

    output [31:0] ALURes
);

wire [31:0] PCPlus4i;       assign PCPlus4i = newPCo + 4;
wire [31:0] branchPCi;      assign branchPCi = branchPCDo;
wire branch;                assign branch = branchDo;
wire CLKSlowi;              assign CLKSlowi = CLKSlow;
wire [31:0] newPCo;
PCControlUnit PCControlUnit(
    CLKSlowi,
    PCPlus4i,
    branchPCi,
    branch,
    
    newPCo
);

wire CLKFasti;              assign CLKFasti = CLKFast;
wire [31:0] addri;          assign addri = branchDo == 1 ? 32'bx : newPCo;
wire [31:0] instructiono;
insROM insROM (
    CLKFasti,
    addri,

    instructiono
);

wire [31:0] instructioni;   assign instructioni = instructiono;
wire reseti;                assign reseti = reset;
wire [31:0] instructionIFo;
wire [31:0] PCPlus4IFo;
wire [1:0]  regDstIFo;
wire [1:0]  aluSrcAIFo;
wire [2:0]  aluSrcBIFo;
wire [1:0]  whatToRegIFo;
wire [2:0]  PCSelIFo;
wire [3:0]  ALUOpIFo;
wire [1:0]  readModeIFo;
wire [1:0]  moveIFo;
IFReg IFReg (
    CLKSlowi,
    PCPlus4i,
    instructioni,
    reseti,

    PCPlus4IFo,
    instructionIFo,

    regDstIFo,
    aluSrcAIFo,
    aluSrcBIFo,
    whatToRegIFo,
    regWriteIFo,
    memWriteIFo,
    PCSelIFo,
    bneIFo,
    ALUOpIFo,
    immExpandIFo,
    readModeIFo,
    moveIFo
);

wire [15:0] numi;           assign numi = instructionIFo[15:0];
wire extModei;              assign extModei = immExpandIFo;
wire [31:0] extDatao;
signExt signExt (
    numi,
    extModei,

    extDatao
);

wire regWritei;             assign regWritei = regWriteWBo;
wire [1:0] readi;           assign readi = moveIFo;
wire [1:0] movei;           assign movei = moveWBo;
wire [4:0] rsi;             assign rsi = instructionIFo[25:21];
wire [4:0] rti;             assign rti = instructionIFo[20:16];
wire [31:0] WDatai;         assign WDatai = WDatao;
wire [4:0] WAddri;          assign WAddri = WAddrWBo;
wire [31:0] WLoi;           assign WLoi = WLoWBo;
wire [31:0] rsDatao;
wire [31:0] rtDatao;
regFile regFile (
    CLKSlowi,
    regWritei,
    readi,
    movei,

    rsi,
    rti,
    WDatai,
    WAddri,
    WLoi,

    rsDatao,
    rtDatao
);

wire [31:0] branchPCDi;     assign branchPCDi = PCPlus4IFo + (extDatao << 2);
wire [31:0] jmpPCDi;        assign jmpPCDi = {PCPlus4IFo[31:28],instructionIFo[25:0],2'b00};
wire [31:0] jrPCDi;         assign jrPCDi = rsDatao;
wire [31:0] resetPCDi;      assign resetPCDi = resetPC;
wire [2:0] PCSelDi;         assign PCSelDi = PCSelIFo;
wire bneDi;                 assign bneDi = bneIFo;
wire [31:0] in1Di;          assign in1Di = rsDatao;
wire [31:0] in2Di;          assign in2Di = rtDatao;
wire [31:0] branchPCDo;
PCDecisionUnit PCDecisionUnit (
    branchPCDi, jmpPCDi, jrPCDi, resetPCDi,
    PCSelDi, bneDi,
    in1Di, in2Di,
    branchPCDo, branchDo
);

wire [31:0] PCPlus4EXi;     assign PCPlus4EXi = PCPlus4IFo;
wire [31:0] opAi;           assign opAi = rsDatao;
wire [31:0] opBi;           assign opBi = rtDatao;
wire [31:0] extImmi;        assign extImmi = extDatao;
wire [31:0] instructionEXi; assign instructionEXi = instructionIFo;
wire [1:0] regDstEXi;       assign regDstEXi = regDstIFo;
wire [1:0] aluSrcAEXi;      assign aluSrcAEXi = aluSrcAIFo;
wire [2:0] aluSrcBEXi;      assign aluSrcBEXi = aluSrcBIFo;
wire [1:0] whatToRegEXi;    assign whatToRegEXi = whatToRegIFo;
wire regWriteEXi;           assign regWriteEXi = regWriteIFo & ~branchEXo;
wire memWriteEXi;           assign memWriteEXi = memWriteIFo & ~branchEXo;
wire [3:0] ALUOpEXi;        assign ALUOpEXi = ALUOpIFo;
wire [1:0] readModeEXi;     assign readModeEXi = readModeIFo;
wire [1:0] moveEXi;         assign moveEXi = moveIFo;
wire branchEXi;             assign branchEXi = branchDo;
wire [31:0] PCPlus4EXo;
wire [31:0] opAo;
wire [31:0] opBo;
wire [31:0] extImmEXo;
wire [31:0] instructionEXo;
wire [1:0] regDstEXo;
wire [1:0] aluSrcAEXo;
wire [2:0] aluSrcBEXo;
wire [1:0] whatToRegEXo;
wire [3:0] ALUOpEXo;
wire [1:0] readModeEXo;
wire [1:0] moveEXo;
EXReg EXReg (
    CLKSlowi,
    PCPlus4EXi,
    opAi, opBi, extImmi,
    instructionEXi,
    regDstEXi,
    aluSrcAEXi,
    aluSrcBEXi,
    whatToRegEXi,
    regWriteEXi,
    memWriteEXi,
    ALUOpEXi,
    readModeEXi,
    moveEXi,
    branchEXi,

    PCPlus4EXo,
    opAo, opBo, extImmEXo,
    instructionEXo,
    regDstEXo,
    aluSrcAEXo,
    aluSrcBEXo,
    whatToRegEXo,
    regWriteEXo,
    memWriteEXo,
    ALUOpEXo,
    readModeEXo,
    moveEXo,
    branchEXo
);

wire [31:0] rsDatai;        assign rsDatai = opAo;
wire [31:0] rtDatai;        assign rtDatai = opBo;
wire [31:0] extImmMuxi;     assign extImmMuxi = extImmEXo;
wire [4:0] shamti;          assign shamti = instructionEXo[10:6];
wire [1:0] aluSrcAMuxi;     assign aluSrcAMuxi = aluSrcAEXo;
wire [2:0] aluSrcBMuxi;     assign aluSrcBMuxi = aluSrcBEXo;
wire [31:0] in1o;
wire [31:0] in2o;
ALUMux ALUMux (
    rsDatai, rtDatai, extImmMuxi, shamti, 
    aluSrcAMuxi, aluSrcBMuxi,
    in1o, in2o
);

wire [4:0] rtMuxi;          assign rtMuxi = instructionEXo[20:16];
wire [4:0] rdMuxi;          assign rdMuxi = instructionEXo[15:11];
wire [1:0] regDstMuxi;      assign regDstMuxi = regDstEXo;
wire [4:0] WAddro;
regMux regMux (
    rtMuxi, rdMuxi, regDstMuxi,
    WAddro
);

wire [1:0] moveWBFi;        assign moveWBFi = moveWBo;
wire [4:0] dstWBFi;         assign dstWBFi = WAddrWBo;
wire [31:0] ALUResWBFi;     assign ALUResWBFi = ALUResWBo;
wire [31:0] WLoWBFi;        assign WLoWBFi = WLoWBo;
wire [1:0] moveMEMFi;       assign moveMEMFi = moveMEMo;
wire [4:0] dstMEMFi;        assign dstMEMFi = WAddrMEMo;
wire [31:0] ALUResMEMFi;    assign ALUResMEMFi = ALUResMEMo; 
wire [31:0] WLoMEMFi;       assign WLoMEMFi = WLoMEMo;
wire [1:0] moveEXFi;        assign moveEXFi = moveEXo;
wire [4:0] rtEXFi;          assign rtEXFi = instructionEXo[20:16];
wire [4:0] rsEXFi;          assign rsEXFi = instructionEXo[25:21];
wire instructionEXTypei;    assign instructionEXTypei = regDstEXo == 2'b00;
wire [31:0] in1EXFi;        assign in1EXFi = in1o;
wire [31:0] in2EXFi;        assign in2EXFi = in2o;
wire [31:0] in1Fo;          
wire [31:0] in2Fo;
// found a bug here, no forwarding when the instructions ahead are beq/bne 
// but I forgot the possibility
EXForwarding EXForwarding (
    moveWBFi, dstWBFi, ALUResWBFi, WLoWBFi,
    moveMEMFi, dstMEMFi, ALUResMEMFi, WLoMEMFi,
    moveEXFi, rtEXFi, rsEXFi, in1EXFi, in2EXFi, instructionEXTypei,
    in1Fo, in2Fo
);

wire [3:0] ALUOpi;          assign ALUOpi = ALUOpEXo;
wire [5:0] functi;          assign functi = instructionEXo[5:0];
wire [3:0] ALUCtro;
ALUControlUnit ALUControlUnit (
    ALUOpi, functi,
    ALUCtro
);

wire [31:0] in1i;           assign in1i = in1Fo;
wire [31:0] in2i;           assign in2i = in2Fo;
wire [3:0] ALUCtri;         assign ALUCtri = ALUCtro;
wire [31:0] ALUReso;
wire [31:0] loo;
ALU ALU (
    in1i, in2i, ALUCtri,
    ALUReso, loo, zeroo
);

wire [31:0] PCPlus4MEMi;    assign PCPlus4MEMi = PCPlus4EXo;
wire [31:0] ALUResMEMi;     assign ALUResMEMi = ALUReso;
wire [31:0] WLoMEMi;        assign WLoMEMi = loo;
wire [4:0] WAddrMEMi;       assign WAddrMEMi = WAddro;
wire [1:0] whatToRegMEMi;   assign whatToRegMEMi = whatToRegEXo;
wire regWriteMEMi;          assign regWriteMEMi = regWriteEXo;
wire memWriteMEMi;          assign memWriteMEMi = memWriteEXo;
wire [31:0] DataMEMi;       assign DataMEMi = opBo;
wire [1:0] readModeMEMi;    assign readModeMEMi = readModeEXo;
wire [1:0] moveMEMi;        assign moveMEMi = moveEXo;
wire [31:0] PCPlus4MEMo;
wire [31:0] ALUResMEMo;
wire [31:0] WLoMEMo;
wire [4:0] WAddrMEMo;
wire [1:0] whatToRegMEMo;
wire [31:0] DataMEMo;
wire [1:0] readModeMEMo;
wire [1:0] moveMEMo; 
MEMReg MEMReg (
    CLKSlowi,
    PCPlus4MEMi, ALUResMEMi, WLoMEMi,
    WAddrMEMi, whatToRegMEMi, regWriteMEMi, memWriteMEMi, DataMEMi,
    readModeMEMi, moveMEMi,
    
    PCPlus4MEMo, ALUResMEMo, WLoMEMo,
    WAddrMEMo, whatToRegMEMo, regWriteMEMo, memWriteMEMo, DataMEMo,
    readModeMEMo, moveMEMo
);

wire MEMWritei;             assign MEMWritei = memWriteMEMo;
wire [1:0] readModei;       assign readModei = readModeMEMo;
wire [7:0] addrRAMi;        assign addrRAMi = ALUResMEMo[7:0];
wire [31:0] indata;         assign indata = DataMEMo;
wire [31:0] outDataRAMo;
RAM RAM (
    CLKFasti,
    MEMWritei, readModei, addrRAMi, indata,
    outDataRAMo
);
// write or read data to or from RAM

wire [31:0] PCPlus4WBi;     assign PCPlus4WBi = PCPlus4MEMo;
wire [31:0] dataWBi;        assign dataWBi = outDataRAMo;
wire [31:0] ALUResWBi;      assign ALUResWBi = ALUResMEMo;
wire [31:0] WLoWBi;         assign WLoWBi = WLoMEMo;
wire [4:0] WAddrWBi;        assign WAddrWBi = WAddrMEMo;
wire [1:0] whatToRegWBi;    assign whatToRegWBi = whatToRegMEMo;
wire regWriteWBi;           assign regWriteWBi = regWriteMEMo;
wire [1:0] moveWBi;         assign moveWBi = moveMEMo;
wire [31:0] PCPlus4WBo;     
wire [31:0] dataWBo;
wire [31:0] ALUResWBo;
wire [31:0] WLoWBo;
wire [4:0] WAddrWBo;
wire [1:0] whatToRegWBo;
wire [1:0] moveWBo;
WBReg WBReg (
    CLKSlowi,
    PCPlus4WBi, dataWBi, ALUResWBi, WLoWBi, 
    // data for regWrite
    WAddrWBi, whatToRegWBi, regWriteWBi, moveWBi, 
    // control signals for regWrite
    PCPlus4WBo, dataWBo, ALUResWBo, WLoWBo,
    WAddrWBo, whatToRegWBo, regWriteWBo, moveWBo
);

wire [31:0] memDatai;       assign memDatai = dataWBo;
wire [31:0] ALUResi;        assign ALUResi = ALUResWBo;
wire [31:0] PCi;            assign PCi = PCPlus4WBo;
wire [1:0] whatToRegi;      assign whatToRegi = whatToRegWBo;
wire [31:0] WDatao; 
regDataMux regDataMux (
    memDatai, ALUResi, PCi,
    whatToRegi,
    WDatao
);
// to select data for reg writing 

endmodule