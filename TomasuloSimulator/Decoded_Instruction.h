#ifndef DECODED_INSTRUCTION_H
#define DECODED_INSTRUCTION_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace std;

struct operand{
public:
  int value;
  bool is_immediate;
  
  operand(int value, bool is_immediate);
};

struct Decoded_Instruction{
public:
  string opcode;
  operand *ops; 
  
  Decoded_Instruction(string s);
};

#endif