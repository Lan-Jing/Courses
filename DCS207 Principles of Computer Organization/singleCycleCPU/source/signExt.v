module signExt (
    input [15:0] num,
    input extMode,

    output [31:0] outData
);

reg [31:0] outData;
// all values that will be assigned in always blocks must be 
// explicitly specified as reg type

always @ ( * )
begin
    if(extMode == 0 || (extMode == 1 && num[15:15] == 0))
        outData = {16'b0,num};
    else 
        outData = {16'hffff,num};
end

endmodule 