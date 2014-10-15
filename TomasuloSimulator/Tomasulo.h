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

#define PROGRAM_FILE_NAME "program.txt"
#define LATENCY_FILE_NAME "Latencies.txt"
#define REGISTER_FILE_NAME "Regster_File.txt"
#define MEMORY_FILE_NAME "Memory_File.txt"

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
  
  /**
   * The current execution cycle.
   */
  int cycle;
  
  /**
   * Flag to be set if debugging info is reqd.
   */
  bool debug;

  /*
   * Instruction window size.
   */
  int max_instruction_buffer_size ;
  
  /*
   *Fetched instructions are kept here at the end of FETCH STAGE 
   */
  queue<string> *instruction_buffer; 

  /*
   * Has all instructions of the program.
   */
  queue<string> *instruction_cache;

  /*
   * Elements of the Tomasulo - ARF, RRF, Reservation Station, ROB, ALUs, LSU
   */
  ARF *arf;
  RRF *rrf;
  Res_Station *rs;
  ROB *rob;
  ALU *alu;
  LSU *lsu;
  
  /*
   * Memory modelled as array of integers.
   */
  int Memory[MAX_MEM+1];
  
  /*
   * Info about execution time of each opcode.
   */
  int instruction_cycles[NUM_INSTRUCTION_TYPES];
  
public:
  
  /*
   * The store index --> unique id given to stores in a program, based on program order.
   * Starts from 0.
   */
  int store_index;
  
  /**
   * Constructor.
   */
  Tomasulo(int num_arch_reg, int num_renamed_reg,int num_rs_entries,int issue_size, ROB *rob,Res_Station *rs, ARF *arf, RRF *rrf, bool debug);
  
  /**
   * Fetch all instructions to the instruction_cache.
   * Note : This is done only once - at the start of simulation.
   */
  void fetch_instructions_to_cache(); 
  
  /**
   * Fetch instructions from i-cache to the instruction buffer.
   * This is done at Stage 2 : Fetch of the pipeline.
   */
  void fetch_instructions_to_buffer(); 
  
  /**
   * Parse the instruction in the instruction buffer and find out required fields.
   * Done at decode stage 
   */
  void decode_instructions();

  /**
   * Models Stage 3 : Execute of the pipeline.
   */
  void execute_instructions();

  /**
   * Commit finished instructions.
   * Models Stage 5 of the pipeline.
   * 
   * @return True if instructions were committed, false otherwise.
   */
  bool commit_instructions(); 

  /*
   * Simulate execution of file PROGRAM_FILE_NAME
   */
  void simulate(); 

  /*
   * Display architectural register file (ARF) 
   */
  void display_arf(); 
  
  /* 
   *Display renamed register file 
   */
  void display_rrf();
  
  /**
   * Display contents of reservation station.
   */
  void display_rs();

  /**
   * Convert opcode in string format to corresponding number.
   * 
   * @param opcode : Opcode specified as a String
   * @return Integer corresponfing to opcode.
   */
  int opcode_helper(string opcode); 

  /**
   * Initialize the Register file with entries specified in REGISTER_FILE_NAME
   */
  void initialize_register_file(); 

  /**
   * Initialize the Register file with entries specified in MEMORY_FILE_NAME
   */
  void initialize_memory();

  /**
   * Initialize the instruction_cycles array using the file LATENCY_FILE_NAME
   */
  void initialize_instruction_cycles();
  
  /**
   * Print out all 100 memory cells.
   */
  void print_memory();
  
  /**
   * Print out contents of 1 specific memory cell.
   * 
   * @param index Memory location of cell to print
   */
  void print_memory(int index);

};

#endif
