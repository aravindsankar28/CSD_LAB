#ifndef RRF_H
#define RRF_H

#include <stdint.h>
#include <iostream>

using namespace std;

struct RRF_Entry{
  
   // index or number of the register
  int index;
  
  // opcode of instruction that has been seen
  int data;

 //busy bit
  int busy;
  
  //valid bit
  int valid;
};

class RRF{
  bool debug;
  int size;
public:
  RRF_Entry *entries;
  RRF(int size, bool debug);
  int find_non_busy_register();
  RRF_Entry* get_entry(int index);
  void display();
};

 
#endif