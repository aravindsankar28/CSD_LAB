#ifndef ALU_H
#define ALU_H

#include <string>
#include "ROB.h"
#include "RRF.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define AND 4
#define OR 5
#define XOR 6

using namespace std;

class ALU{
  int curr_opcode;
  int reqd_cycles;
  int curr_cycle;
  bool is_busy;
  int src1;
  int src2;
  int dest;
  int scratch;
  
  ROB* rob;
  RRF* rrf;
  bool commit;
  int calculate();
  
public:
  /**
   * Issue an instruction to the ALU.
   * @param opcode : opcode of operation to be executed
   * @param src1   : Reanmed register corr to src 1
   * @param src2   : Reanmed register corr to src 2
   * @param dest   : Reanmed register corr to destiation
   */
  void issue_instruction(int opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf);
  
  /**
   * Commit result of instruction to ROB/RRF
   */
  void commit();
  
  /**
   * Run the ALU for this cycle.
   * Executed once every cycle.
   */
  void run();
};
  
#endif
