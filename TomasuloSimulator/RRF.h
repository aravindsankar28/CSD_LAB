#ifndef RRF_H
#define RRF_H


#include <stdint.h>
struct RRF_Entry{
  
  // opcode of instruction that has been seen
  uint64_t data;

 //busy bit
  int busy;
  
  //valid bit
  int valid;
};

class RRF{
  int size;
  RRF_Entry *entries;

public:
  RRF(int size);
};

#endif