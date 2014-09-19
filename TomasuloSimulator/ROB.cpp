#include "ROB.h"

ROB(int max_size)
{
  this->max_size = max_size;
  entries = new queue<ROB_Entry>();
}

bool ROB::attempt_push(ROB_Entry r)
{
  if (this->entries.size() < max_size){
    this->entries.push_back(r);
    return true;
  }
  return false;
}

void ROB::attempt_pop()
{
  if (this->entries.empty())
    return false;
  
  ROB_Entry re = this->entries.front();
  if(re.exec){
    this->entries.pop_front();
    //TODO: Need to inform RRF here
    return true;
  }
  else
    return false;
}

void ROB::set_complete(int tag)
{
  for(int i = 0; i < this->entries.size(); i++){
    if(this->entries[i].tag == tag){
      this->entries[i].exec = true;
    }
  }
}

