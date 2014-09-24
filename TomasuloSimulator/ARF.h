#ifndef ARF_H
#define ARF_H


#include <stdint.h>


struct ARF_Entry{

  // index or number of the register
  int index;

  //contents of the register
  uint64_t data;
  
  //index of entry in RRF that corresponds to this register
  int tag;
  
  //busy bit
  bool busy;
};

class ARF{
  int size;
 
  
public:
  ARF_Entry *entries;
  ARF_Entry get_entry(int index);
  ARF(int size);
  //All member functions here
};

#endif
