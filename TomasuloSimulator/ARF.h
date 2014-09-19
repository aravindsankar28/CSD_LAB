#ifndef ARF_H
#define ARF_H

#include <cstdint>

struct ARF_Entry{
  //contents of the register
  uint64_t data;
  
  //index of entry in RRF that corresponds to this register
  int tag;
  
  //busy bit
  bool busy;
};

class ARF{
  int size;
  ARF_Entry *entries;
  
public:
  
  ARF(int size);
  //All member functions here
};

#endif