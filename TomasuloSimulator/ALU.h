#ifndef ALU_H
#define ALU_H

#include "Tomasulo.h"
class ALU{
  int curr_opcode;
  int reqd_cycles;
  int curr_cycle;
  
  int src1;
  int src2;
  int dest;
  
public:
  void issue_instruction(int opcode, int src1, int src2, int dest);
  void commit();
  void run();
  
};
  
#endif