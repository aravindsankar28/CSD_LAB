#ifndef RRF_H
#define RRF_H

struct RRF_Entry{
  
  // opcode of instruction that has been seen
  uint64_t data;

 /*  
  //ARF registers corresponding to required operands
  int src1;
  int src2;
  
  //ARF registers corresponding to required sink
  int dest;
 */  

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