#ifndef ARF_H
#define ARF_H


#include <stdint.h>
#include <iostream>

using namespace std;
struct ARF_Entry{

  // index or number of the register
  int index;

  //contents of the register
  int data;
  
  //index of entry in RRF that corresponds to this register
  int tag;
  
  //busy bit
  bool busy;
};

class ARF{
 bool debug;
public:
  int size;
  ARF_Entry *entries;
  ARF_Entry* get_entry(int index);
  ARF(int size, bool debug);
  void display();
  //All member functions here
};

 
#endif