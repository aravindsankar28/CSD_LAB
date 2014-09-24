#include "Decoded_Instruction.h"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

Decoded_Instruction::Decoded_Instruction(string s)
{
  this->ops = (operand*) malloc(3*sizeof(operand));
  
  string tmp;
  vector <string> cds;
  stringstream ss (s);
  int i = 0;
  
  while(ss >> tmp)
  {
    if( i == 0)
    {
       this->opcode  = tmp;
       i++;
       continue;
    }
    if(tmp[0] == 'R')
    {
       int value = atoi(tmp.substr(1).c_str());    
       this->ops[i-1] = *(new operand(value,false));
    }
    else
    {
       int value = atoi(tmp.c_str());    
       this->ops[i-1] = *(new operand(value,true));
    } 
    i++;
  }
}

operand::operand(int value, bool is_immediate)
{
  this->value = value;
  this->is_immediate = is_immediate;
}

