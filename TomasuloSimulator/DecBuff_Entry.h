#ifndef DEC_BUFF_ENTRY
#define DEC_BUFF_ENTRY

#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct operand{
public:
  int value;
  bool is_immediate;
  operand(int value, bool is_immediate);
};

struct DecBuff_Entry{
public:
  string opcode;
  operand *ops; 
  
  DecBuff_Entry(string s);
};

#endif