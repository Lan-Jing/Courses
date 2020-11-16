`timescale 1 ns / 1 ps
module RAM(
    input CLK,
    input MEMWrite,
    input [1:0] readMode,
    input [7:0] addr,
    input [31:0] indata,

    output [31:0] outData
);

reg         reset;
reg  [31:0] outData;
reg  [31:0] tempDatai;
wire [31:0] tempDatao;

wire MEMWrite1;
wire MEMWrite2;
wire MEMWrite3;
wire MEMWrite4;

assign MEMWrite4 = MEMWrite & (
                   (readMode == 2'b10 | readMode == 2'b11)  | 
                   (readMode == 2'b00 & addr[1:0] == 2'b11) | 
                   (readMode == 2'b01 & addr[1:0] == 2'b10));
assign MEMWrite3 = MEMWrite & ((readMode == 2'b10 | readMode == 2'b11) | (readMode == 2'b00 & addr[1:0] == 2'b10) | (readMode == 2'b01 & addr[1:0] == 2'b10));
assign MEMWrite2 = MEMWrite & ((readMode == 2'b10 | readMode == 2'b11) | (readMode == 2'b00 & addr[1:0] == 2'b01) | (readMode == 2'b01 & addr[1:0] == 2'b00));
assign MEMWrite1 = MEMWrite & (addr[1:0] == 2'b00);

dataRAM RAM1 (CLK,~reset,1'b1,MEMWrite1,addr[7:2],tempDatai[7:0],tempDatao[7:0]); // low address
dataRAM RAM2 (CLK,~reset,1'b1,MEMWrite2,addr[7:2],tempDatai[15:8],tempDatao[15:8]);
dataRAM RAM3 (CLK,~reset,1'b1,MEMWrite3,addr[7:2],tempDatai[23:16],tempDatao[23:16]);
dataRAM RAM4 (
    CLK,~reset,1'b1,MEMWrite4,
    addr[7:2], tempDatai[31:24], 
    tempDatao[31:24]); // high address
// small-end mode, LSB on the lowest address

initial begin
    reset <= 1;
    #10000;
    reset <= 0;
end

always @ ( * )
begin
    casex (readMode)
        2'b00: 
            case (addr[1:0]) 
                2'b00: tempDatai = {24'b0,indata[7:0]};
                2'b01: tempDatai = {16'b0,indata[7:0],8'b0};
                2'b10: tempDatai = {8'b0,indata[7:0],16'b0};
                2'b11: tempDatai = {indata[7:0],24'b0};
            endcase
        2'b01: begin
            case (addr[1:0])
                2'b00: tempDatai = {16'b0,indata[15:0]};
                2'b10: tempDatai = {indata[15:0],16'b0};
                default: tempDatai = 32'b0;
            endcase
        end
        2'b1x: begin
            tempDatai = indata;
        end
    endcase
end

always @ ( * )
begin
    if(readMode == 2'b00) // read single byte
    begin
        casex (addr[1:0])
            2'b11: outData = {{24{tempDatao[31]}},tempDatao[31:24]};
            2'b10: outData = {{24{tempDatao[23]}},tempDatao[23:16]};
            2'b01: outData = {{24{tempDatao[15]}},tempDatao[15:8]};
            2'b00: outData = {{24{tempDatao[7]}},tempDatao[7:0]};
        endcase
    end
    else if (readMode == 2'b11 || readMode == 2'b10) // read word
        outData = tempDatao;
    else begin
        case (addr[1:0])
            2'b10: outData = {{16{tempDatao[31]}},tempDatao[31:16]};
            2'b00: outData = {{16{tempDatao[15]}},tempDatao[15:0]};
            default: outData = 32'hxxxxxxxx;
        endcase
    end // read half-word
end

endmodule