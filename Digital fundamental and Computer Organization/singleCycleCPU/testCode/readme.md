# Final Project for DCS207 Principles of Computer Organization: Simple Implementation of MIPS CPU on FPGA Platform.

* Single-cycle CPU
  * instructions from ROM
  * Data stored from/loaded to RAM
  * PC controller, jmp/jr are supported.
  * Decode unit.
  * ALU supporting most operations, multiplication and so on.
* Pipeline CPU
  * Reuse most subsystems.
  * Pipeline registers added between stages.
  * Forwarding is supported.
  * Simplest branch prediction: Flush the pipeline with a signal.