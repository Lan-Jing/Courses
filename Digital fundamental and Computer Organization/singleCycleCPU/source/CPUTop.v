module CPU (
    input CLKFast,
    input CLKSlowed,      
    input reset,          // reset signal for starting CPU
    input [31:0] resetPC, // the initial PC location

    output [31:0] ALURes
);

wire [31:0] PCPlus4i;
wire [31:0] branchPCi;
wire [31:0] jmpPCi;
wire [31:0] jrPCi;
wire [31:0] resetPCi;
wire [31:0] PCSeli;
wire bnei;
wire zeroi;
wire CLKi;
wire [31:0] PCo;
PCControlUnit PCControlUnit (
    PCPlus4i, branchPCi, jmpPCi, jrPCi, resetPCi, PCSeli,
    bnei, zeroi,
    CLKi,
    PCo
);

wire [25:0] Jinstructioni;
wire [31:0] immediatei;
wire [31:0] oldPCi;
wire [31:0] PCPlus4o;
wire [31:0] branchPCo;
wire [31:0] jmpPCo;
PCCalUnit  PCCalUnit (
    Jinstructioni, immediatei, oldPCi,
    PCPlus4o, branchPCo, jmpPCo
);

wire CLKiR;
wire [31:0] ROMaddri;
wire [31:0] instructiono;
insROM insROM (
    CLKiR,
    ROMaddri,
    instructiono
);

wire [4:0] rti;
wire [4:0] rdi;
wire [1:0] regDsti;
wire [4:0] WAddro;
regMux RegMux (
    rti, rdi,
    regDsti,
    WAddro
);

wire regWritei;
wire [1:0] movei;
wire [4:0] rsi;
wire [31:0] WDatai;
wire [4:0] WAddri;
wire [31:0] Wloi;
wire [31:0] rsDatao;
wire [31:0] rtDatao;
regFile regFile (
    CLKi, regWritei, movei,
    rsi, rti, WDatai, WAddri, Wloi,
    rsDatao, rtDatao
);

wire [15:0] numi;
wire extModei;
wire [31:0] ExtDatao;
signExt signExt (
    numi, 
    extModei,
    ExtDatao
);

wire reseti;
wire [5:0] opCodei;
wire [5:0] functi;
wire [1:0] regDsto;
wire [1:0] aluSrcAo;
wire [2:0] aluSrcBo;
wire [1:0] whatToRego;
wire regWriteo;
wire memWriteo;
wire [2:0] PCSelo;
wire bneo;
wire [3:0] ALUOpo;
wire immExpando;
wire [1:0] readModeo;
wire [1:0] moveo;
controlUnit controlUnit (
    opCodei, functi, reseti,
    regDsto,
    aluSrcAo,
    aluSrcBo,
    whatToRego,
    regWriteo,
    memWriteo,
    PCSelo,
    bneo,
    ALUOpo,
    immExpando,
    readModeo,
    moveo
);

wire [31:0] PCi;
wire [31:0] outDatai;
wire [31:0] ALUResi;
wire [1:0] whatToRegi;
wire [31:0] WDatao;
regDataMux regDataMux (
    PCi, outDatai, ALUResi,
    whatToRegi,
    WDatao
);

wire memWritei;
wire [1:0] readModei;
wire [7:0] RAMaddri;
wire [31:0] inDatai;
wire [31:0] outDatao;
RAM RAM (
    CLKiR, memWritei,
    readModei, RAMaddri, inDatai, outDatao
);

wire [31:0] in1i;
wire [31:0] in2i;
wire [3:0] ALUCtri;
wire [31:0] ALUReso;
wire [31:0] loo;
wire zeroo;
ALU ALU (
    in1i, in2i,
    ALUCtri,
    ALUReso, loo, zeroo
);

wire [31:0] rsDatai;
wire [31:0] rtDatai;
wire [31:0] extImmi;
wire [4:0] shamti;
wire [1:0] aluSrcAi;
wire [2:0] aluSrcBi;
wire [31:0] in1o;
wire [31:0] in2o;
ALUMux ALUMux (
    rsDatai, rtDatai, extImmi, shamti,
    aluSrcAi, aluSrcBi,
    in1o, in2o
);

wire [3:0] ALUOpi;
wire [3:0] ALUCtro;
ALUControlUnit ALUControlUnit (
    ALUOpi, functi, ALUCtro
);

// assignment on PCControlUnit
assign PCPlus4i = PCPlus4o;
assign branchPCi = branchPCo;
assign jmpPCi = jmpPCo;
assign jrPCi = ALUReso;
assign resetPCi = resetPC;
assign PCSeli = PCSelo;
assign bnei = bneo;
assign zeroi = zeroo;
assign CLKi = CLKSlowed;
// assignment on PCCaulUnit
assign Jinstructioni = instructiono[25:0];
assign immediatei = ExtDatao;
assign oldPCi = PCo;
// assignment on insROM
assign CLKiR = CLKFast;
assign ROMaddri = PCo;
// assignment on RegMux
assign rti = instructiono[20:16];
assign rdi = instructiono[15:11];
assign regDsti = regDsto;
// assignment on regFile
assign reseti = reset;
assign regWritei = regWriteo;
assign movei = moveo;
assign rsi = instructiono[25:21];
assign WDatai = WDatao;
assign WAddri = WAddro;
assign Wloi = loo;
// assignment on signExt
assign numi = instructiono[15:0];
assign extModei = immExpando;
// assignment on controlUnit
assign opCodei = instructiono[31:26];
assign functi = instructiono[5:0];
// assignment on regDataMux 
assign PCi = PCo;
assign outDatai = outDatao;
assign ALUResi = ALUReso;
assign whatToRegi = whatToRego;
// assignment on RAM
assign memWritei = memWriteo;
assign readModei = readModeo;
assign RAMaddri = ALUReso;
assign inDatai = rtDatao;
// assignment on ALU
assign in1i = in1o;
assign in2i = in2o;
assign ALUCtri = ALUCtro;
// assignment on ALUMux 
assign rsDatai = rsDatao;
assign rtDatai = rtDatao;
assign extImmi = ExtDatao;
assign shamti = instructiono[10:6];
assign aluSrcAi = aluSrcAo;
assign aluSrcBi = aluSrcBo;
// assignment on PCControlUnit
assign ALUOpi = ALUOpo;

// assignment on display input 
assign ALURes = ALUReso;

endmodule