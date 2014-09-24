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


using namespace std;



/**
 * Modelled the registers as 64-bit integers
 */
//typedef reg uint64_t;

class Tomasulo{
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
  
  int max_instruction_buffer_size ;
  /*Fetched instructions are kept here at the end of FETCH STAGE */ 

  queue<string> *instruction_buffer;


  queue<string> *instruction_cache; // has all instructions

  queue<string>* fetchBuff;
  
  /**
   * Buffer between decode-I and decode-II
   */
  //queue<Decoded_Instruction> decodeBuff;
  
  /**
   * Elements of the algorithm
   */  
  ARF *arf;
  RRF *rrf;
  Res_Station *rs;
  ROB *rob;
  
public:
  Tomasulo(int num_arch_reg, int num_renamed_reg,int num_rs_entries,int issue_size, ROB *rob,Res_Station *rs, ARF *arf, RRF *rrf)
  {
    this->num_arch_reg = num_arch_reg;
    this->num_renamed_reg= num_renamed_reg;
    this->num_rs_entries = num_rs_entries;
    this->issue_size = issue_size;
    this->rob = rob;
    this->rs = rs;
    this->arf = arf;
    this-> rrf = rrf;
    this->instruction_cache = new queue<string>();
    this->instruction_buffer = new queue<string>();
    this->max_instruction_buffer_size = 4;
  }

// done only once
  void fetch_instructions_to_cache();
  
  // done at fetch stage
  void fetch_instructions_to_buffer();
  
  void decode_instructions();

  // Needs to be done before the simulation starts.
  void initialize_register_file(); 
};

  /**
  * Get number of cycles reqd fr execution of opcode.
  */
  int get_cycles(int opcode);



#endif
