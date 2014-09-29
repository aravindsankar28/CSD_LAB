#include "ALU.h"

#include "Tomasulo.h"
#include <stdint.h>

void ALU::issue_instruction(int instruction_number,int opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf )
{
  this->instruction_number = instruction_number;
  this->curr_opcode = opcode;
  //this->reqd_cycles = get_cycles(opcode);
  this->reqd_cycles = 1; // For now
  this->curr_cycle = 0;
  this->is_busy = true;
  this->commited = false;
  this->src1 = src1;
  this->src2 = src2;
  this->dest = dest;
  this->rob = rob;
  this->rrf = rrf;
  cout << "instruction " << this->instruction_number << " issued on ALU "<<endl;
}

void ALU::commit()
{
  /*TODO: Commit instruction
   * Set the bit in ROB
   * Change valid bit and data field of RRF entry
   */
  if(this->commited)
    {

      this->rob->set_complete(this->dest);
      RRF_Entry *rrf_entry = this->rrf->get_entry(this->dest);
      rrf_entry->valid = true;
      this->is_busy = false;
      this->commited = false;
      rrf_entry->valid = true;

      rrf_entry->data = this->scratch;  
      cout << "ALU Commit done of instruction "<< this->instruction_number<<endl;
      
    }
}

void ALU::run()
{
  this->curr_cycle++;
  if(this->is_busy && this->curr_cycle == this->reqd_cycles){
    this->scratch = this->calculate();
    this->commited = true;
    cout << "Finished calculation of instruction "<<this->instruction_number<<endl;
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
