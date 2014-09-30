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
    cout << "Pushing to rob instruction "<< r.instruction_number <<endl;
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
  if(re.exec)
  {

      while(this->entries.size() >0 && re.exec ){
        re = this->entries.front();
        cout << "Popping from rob to commit and retire instruction "<< re.instruction_number <<endl;
        //this->scratch = this->entries.front();
        this->scratch_queue.push_back(this->entries.front());
        this->entries.pop_front();
        
      }
     
      return true;
  }
  else
    return false;

}

void ROB::set_complete(int tag)
{

  for(int i = 0; i < this->entries.size(); i++){
    
    if(this->entries[i].tag == tag){
      cout << "Setting exec to true for instruction "<< this->entries[i].instruction_number <<endl;
      this->entries[i].exec = true;
    }
  }

}

void ROB::set_complete_ip(int instruction_number)
{
  for(int i = 0; i < this->entries.size(); i++){
    
    if(this->entries[i].instruction_number == instruction_number){
      cout << "Setting exec to true for instruction "<< this->entries[i].instruction_number <<endl;
      this->entries[i].exec = true;
    }
  }
}

int ROB::get_size()
{
  return this->entries.size();
}

