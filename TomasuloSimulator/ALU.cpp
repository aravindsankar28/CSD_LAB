#include "ALU.h"

#include <stdint.h>

void ALU::issue_instruction(int opcode, int src1, int src2, int dest)
{
  this->curr_opcode = opcode;
  this->reqd_cycles = get_cycles(opcode);
  this->curr_cycle = 0;
  
  this->src1 = src1;
  this->src2 = src2;
  this->dest = dest;
}

void ALU::commit()
{
  /*TODO: Commit instruction
   * Set the bit in ROB
   * Change busy bit and data field of RRF entry
   */
  
}

void ALU::run()
{
  this->curr_cycle++;
  if(this->curr_cycle == this->reqd_cycles)
    this->commit();
}

