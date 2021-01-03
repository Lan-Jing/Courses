# Final Project for DCS207 Principles of Computer Organization: MIPS CPU on FPGA platform.

* Single-cycle CPU
  * PC controller
  * Arithmetic controller
  * ALU unit
  * Instructions from ROM
  * Data stored from/loaded to RAM
  * Simulation on each unit to ensure expected functionality.
* Pipeline CPU
  * Reuse most components.
  * Add registers between two stages.
  * Support most instructions, including jmp/jr/mul an so on.
  * For jump instructions, use a signal to flush the pipeline.
  * Support forwarding.