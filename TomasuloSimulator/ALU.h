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
  int instruction_number;
  ROB* rob;
  RRF* rrf;
  bool commited;
  bool debug;
  int calculate();
  
public:
  int* instruction_cycles;
  bool is_busy;
  void issue_instruction(int instruction_number,int opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf);
  
  /**
   * Commit result of instruction to ROB and RRF
   */
  void commit();
  
  /**
   * Run the ALU for this cycle.
   * Executed once every cycle.
   */
  void run();

  ALU(bool debug)
  {
    commited = false;
    this->debug = debug;
  }
};
  
 
#endif