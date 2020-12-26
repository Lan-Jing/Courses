`timescale 1 ns/ 1 ps
`define cycleTime 10
module insROMSim();

reg [31:0]   addr;
reg          CLK;
wire [31:0]  instruction; 
// the output instruction must be of wire type
initial 
begin
    addr <= 0;
    CLK <= 0;
end

always # (`cycleTime / 2) // use '`' to declare it a micro
begin
    CLK <= !CLK;
end

insROM ROM (CLK, addr, instruction);   
// put the module instantiation out of always block  
always @ (posedge CLK)
begin
    if(addr < 19 * 4)
        addr <= addr + 4;
    else
        addr <= 0;
end

endmodule