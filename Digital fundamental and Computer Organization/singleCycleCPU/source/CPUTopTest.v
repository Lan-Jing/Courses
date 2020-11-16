`timescale 1 ns / 1 ps 
module CPUTopTest(
    input CLK,
    input res,
    input dispSwitch,

    output [3:0] segCode,
    output [6:0] seg
);

reg CLK_slow;
reg [31:0] CLK_slowCount;

initial begin
    CLK_slowCount <= 0;
    CLK_slow <= 0;
end

always @ (posedge CLK) begin
    if(CLK_slowCount == 75_000000) begin
        CLK_slowCount <= 0;
        CLK_slow <= ~CLK_slow;
    end
    else begin
        CLK_slowCount <= CLK_slowCount + 1;
    end
end

wire [31:0] outData;
wire [15:0] dispIndata;
wire reseti;

assign reseti = res;
assign dispIndata = dispSwitch == 1 ? outData[31:16] : outData[15:0];

CPU CPU (CLK,CLK_slow, reseti, 32'b0, outData);
display disp (CLK,dispIndata,segCode,seg);

endmodule 