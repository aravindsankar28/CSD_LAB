#ifndef TOMASULO_H
#define TOMASULO_H

#include <stdint.h>
#include <string>
#include <queue>

class RRF;
class ARF;
class ROB;
class Res_Station;
class DecBuff_Entry;

using namespace std;

// #include "Combined.h"

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
  
  /**
   * Buffer between fetch and decode-I stage
   */
  queue<string>* fetchBuff;
  
  /**
   * Buffer between decode-I and decode-II
   */
  queue<DecBuff_Entry> decodeBuff;
  
  /**
   * Elements of the algorithm
   */  
  ARF *arf;
  RRF *rrf;
  Res_Station *rs;
  ROB *rob;
  
public:
  string* fetch_instructions();
  
  //DecBuff_Entry* decode_instruction(string);
};

  /**
  * Get number of cycles reqd fr execution of opcode.
  */
  int get_cycles(int opcode);



#endif
