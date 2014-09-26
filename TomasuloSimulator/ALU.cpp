#include "ALU.h"

#include "Tomasulo.h"
#include <stdint.h>

void ALU::issue_instruction(string opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf )
{
  this->curr_opcode = opcode;
  this->reqd_cycles = get_cycles(opcode);
  this->curr_cycle = 0;
  this->is_busy = true;
  this->src1 = src1;
  this->src2 = src2;
  this->dest = dest;
  this->rob = rob;
  this->rrf = rrf;
}

void ALU::commit()
{
  /*TODO: Commit instruction
   * Set the bit in ROB
   * Change valid bit and data field of RRF entry
   */
  this->rob->set_complete(this->dest);
  RRF_Entry *re = this->rrf->get_entry(this->dest);
  re->valid = false;
  
}

void ALU::run()
{
  this->curr_cycle++;
  if(this->curr_cycle == this->reqd_cycles){
    this->calculate();
    this->commit();
  }
}


int ALU::calculate()
{
  int s1 = this->src1;
  int s2 = this->src2;
  
  switch(curr_opcode){
    case ADD:
      return s1 + s2;
    case SUB:
      return s1 - s2;
    case MUL:
      return s1 * s2;
    case DIV:
      return s1 / s2;
    case AND:
      return s1 & s2;
    case OR:
      return s1 | s2;
    case XOR:
      return s1 ^ s2;
  }
}
