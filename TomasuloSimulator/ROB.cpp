#include "ROB.h"

ROB_Entry::ROB_Entry(int tag, bool exec)
{
  this->tag = tag;
  this->exec = exec;
}

ROB_Entry::ROB_Entry()
{
  this->tag = -1;
  this->exec = false;
}

ROB::ROB(int max_size)
{
  this->max_size = max_size;
  entries = *(new deque<ROB_Entry>());

}

bool ROB::attempt_push(ROB_Entry r)
{
  if (this->entries.size() < max_size){
    this->entries.push_back(r);
    return true;
  }
  return false;
}

bool ROB::attempt_pop()
{
  if (this->entries.empty())
    return false;
  
  ROB_Entry re = this->entries.front();
  if(re.exec){
    cout << "Popping from rob to commit and retire" <<endl;
    this->scratch = this->entries.front();
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

int ROB::get_size()
{
  return this->entries.size();
}

