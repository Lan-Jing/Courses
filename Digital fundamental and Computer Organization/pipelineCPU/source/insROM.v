`timescale 1 ns / 1 ps 
module insROM (
    input CLK,
    input [31:0] addr,

    output [31:0] instruction
);

wire [7:0] ins1;
wire [7:0] ins2;
wire [7:0] ins3;
wire [7:0] ins4;

insROM1 insROM1 (CLK,1'b1,{2'b00,addr[7:2]},ins1);
insROM2 insROM2 (CLK,1'b1,{2'b00,addr[7:2]},ins2);
insROM3 insROM3 (CLK,1'b1,{2'b00,addr[7:2]},ins3);
insROM4 insROM4 (CLK,1'b1,{2'b00,addr[7:2]},ins4);

assign instruction = {ins1,ins2,ins3,ins4};

endmodule