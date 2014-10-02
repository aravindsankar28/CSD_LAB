#ifndef TOMASULO_H
#define TOMASULO_H
#include <stdint.h>
#include <string>
#include <queue>
#include <vector>


#include "RRF.h"
#include "ARF.h"
#include "ResStation.h"
#include "Decoded_Instruction.h"
#include "ROB.h"
#include "ALU.h"
#include "LSU.h"

#define MAX_MEM 100
#define NUM_INT_UNITS 2
#define NUM_INSTRUCTION_TYPES 9

using namespace std;

class Tomasulo{

  int curr_instr;
  /**
  * This must be equal to number of entries in ARF
  */
  int num_arch_reg;

  /**
  * This must be equal to number of entries in RRF
  */
  int num_renamed_reg;

  /**
  * This must be the number of entries in reservation station
  */
  int num_rs_entries;

  /**
  * Number of instructions fetched per cycle.
  */
  int issue_size;
  
  int cycle;
  
  bool debug;

  int max_instruction_buffer_size ;
  
  queue<string> *instruction_buffer; /*Fetched instructions are kept here at the end of FETCH STAGE */


  queue<string> *instruction_cache; /* Has all instructions of the program */

    
  ARF *arf;
  RRF *rrf;
  Res_Station *rs;
  ROB *rob;
  int Memory[MAX_MEM+1];
  int instruction_cycles[NUM_INSTRUCTION_TYPES]; // has latencies for each instruction.
  ALU *alu;
  LSU *lsu;
  
public:
  Tomasulo(int num_arch_reg, int num_renamed_reg,int num_rs_entries,int issue_size, ROB *rob,Res_Station *rs, ARF *arf, RRF *rrf, bool debug);
  
  void fetch_instructions_to_cache(); /* Done only once at the start of the program */
  
  
  void fetch_instructions_to_buffer(); /* Done at fetch stage */
  
  void decode_instructions(); /* Done at decode stage */

  void execute_instructions(); /* Done at execute stage */

  bool commit_instructions(); /* Done at commit/retire stage */

  void simulate(); /* Called to sstart the simluation */

  void display_arf(); /* Helper to display arf register file */

  void display_rrf(); /* Helper to display rrf register file */

  void display_rs(); /* Helper to display contents of reservation station */

  int opcode_helper(string opcode); /* Returns opcode no. on input of opcode string */

  // Needs to be done before the simulation starts 
  void initialize_register_file(); 

  void initialize_memory();

  int store_index;
  
  void initialize_instruction_cycles();
  
  void print_memory();
  
  void print_memory(int index);

};

#endif
