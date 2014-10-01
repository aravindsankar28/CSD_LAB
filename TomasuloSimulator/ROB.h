#ifndef ROB_H
#define ROB_H

#include <deque>
#include <iostream>
#include "LSU.h"

using namespace std;


struct ROB_Entry{
public:
  int tag;
  bool exec;
  string opcode;
  int instruction_number;
  int src1;
  int src2;
  

  ROB_Entry(int tag,string opcode);
  ROB_Entry();
  void print();
};


class ROB{

  LSU* lsu;
 
  /**
   * Scratch space to store the popped out entry
   */
  
public:
  deque<ROB_Entry> entries;
  
  /**
   * Constructor.
   * @param max_size	:Size of the ROB
   */
  ROB_Entry scratch;
  deque<ROB_Entry> scratch_queue;
  int max_size;
  ROB(int max_size);
 
  /**
   * Try to push the ROB entry to back of the queue.
   * This will be called during the decode stage.
   * @param r	: Entry to be pushed
   * @return true if success, false if failure
   */
  bool attempt_push(ROB_Entry r);
  
  /**
   * Pop the ROB_Entry at the head of the queue and store it in scratch_space.
   * @return true if success, false if failure
   */
  bool attempt_pop();
  
  ROB_Entry get_scratch_val();
  
  /**
   * Search and mark instruction writing to renamed reg 'tag' as complete.
   * @param tag   : Instruction to be set to executed
   */
  void set_complete(int tag);
  void set_complete_store(int instruction_number);
  int get_size();
  void print();
};

#endif
