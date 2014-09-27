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
#define LOAD 7
#define STORE 8

using namespace std;

class ALU{
  int curr_opcode;
  int reqd_cycles;
  int curr_cycle;
 
  int src1; // value
  int src2; // value
  int dest; // rrf tag
  int scratch;
  
  ROB* rob;
  RRF* rrf;
  bool commited;
  int calculate();
  
public:

  bool is_busy;
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
