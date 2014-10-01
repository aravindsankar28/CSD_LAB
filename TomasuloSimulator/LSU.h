#ifndef LSU_H
#define LSU_H

#include <string>
#include <deque>

#include "RRF.h"

#define LOAD 7
#define STORE 8

using namespace std;

class ROB;

class Store_Queue_Entry{
public:
  int instruction_number;
  int memory_address;
  int data_to_store;
  int has_been_overtaken;
  int latency_timer;
  
  bool completed;

  Store_Queue_Entry()
  {
    this->memory_address = -1;
    this->instruction_number  = -1;
    this->has_been_overtaken =0;
    this->latency_timer = -1;
  }

};


class LSU{


/* REGISTERS FOR STORE INSTRUCTION */
  int reqd_cycles;
  int curr_cycle;  
  int opcode;
  int src1; // value
  int src2; // value
  int dest; // rrf tag
  int scratch;
  int instruction_number;
  bool commited;



/* REGISTERS FOR LOAD MEM INSTRUCTION */

  int reqd_cycles_mem;
  int curr_cycle_mem;  
  int opcode_mem;
  int src1_mem; // value
  int src2_mem; // value
  int dest_mem; // rrf tag
  int scratch_mem;
  int instruction_number_mem;
  bool commited_mem;


/* REGISTERS FOR LOAD FORWARDED INSTRUCTION */
  int reqd_cycles_for;
  int curr_cycle_for;  
  int opcode_for;
  int src1_for; // value
  int src2_for; // value
  int dest_for; // rrf tag
  int scratch_for;
  int instruction_number_for;
  bool commited_for;


  ROB* rob;
  RRF* rrf;
  int *mem;
  bool is_forwarding;
  bool is_store_adding_in_progress;

 
  int calculate();
  
public:

  bool is_busy;
  int store_queue_max_size;
  int* instruction_cycles;
  deque<Store_Queue_Entry> store_queue;

  void issue_instruction(int instruction_number,int opcode, int src1, int src2, int dest, ROB* rob, RRF* rrf, bool is_forwarding);
  int rob_position_recent_store;

  bool is_forwarding_possible(int memory_address);
  int find_val(int memory_address);

  void commit();
  
  /**
   * Run the LSU for this cycle.
   * Executed once every cycle.
   */
  void run();

  void try_pop_from_store_queue();
  LSU(int *mem,int store_queue_max_size)
  {
    this->commited = false;
    this->mem = mem;
    this->is_forwarding = false;
    this->is_store_adding_in_progress = false;
    this->is_busy = false;
    this->commited_mem = false;
    this->store_queue_max_size = store_queue_max_size;
    rob_position_recent_store = -1;
  }
};

  
#endif
