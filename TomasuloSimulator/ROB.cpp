#include "ROB.h"

ROB_Entry::ROB_Entry(int tag, bool exec,string opcode)
{
  this->tag = tag;
  this->exec = exec;
  this->opcode = opcode;
}

ROB_Entry::ROB_Entry()
{
  this->tag = -1;
  this->exec = false;
  this->opcode = "";
  
}

ROB::ROB(int max_size)
{
  this->max_size = max_size;
  entries = *(new deque<ROB_Entry>());
  scratch_queue = *(new deque<ROB_Entry>());
  scratch_queue.clear();
   
}

void ROB_Entry::print()
{
  cout << "Tag :"<<this->tag<<" : "<< "IP :"<<this->instruction_number<<" : "<< "Exec :"<<this->exec << endl;
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
  //cout << this->entries.size()<<endl;
  if (this->entries.empty())
    return false;
//   cout<<"Attempting pop..."<<endl;
 // this->print();
//   cout<<this->entries.size()<<endl;
//   cout<< "SIZE "<<this->scratch_queue.size() <<endl;
  ROB_Entry re = this->entries.front();
  if(re.exec)
  {
      while(this->entries.size() >0 && re.exec ){
	
       
        //this->scratch = this->entries.front();
	cout << "Popping from rob to commit and retire instruction "<<re.instruction_number <<endl;
	this->scratch_queue.push_back(re);
	
        this->entries.pop_front();  
	if(this->entries.size() == 0)
	  break;
	re = this->entries.front();
	
      }
      
//       cout << "pop done."<<endl;
      return true;
  }
  else
    return false;
}

void ROB::set_complete(int tag)
{

  for(int i = 0; i < this->entries.size(); i++){
    
    if(this->entries[i].tag == tag &&  this->entries[i].opcode != "STORE"){
      cout << "Setting exec to true for instruction "<< this->entries[i].instruction_number <<endl;
      this->entries[i].exec = true;
    }
  }

}

void ROB::set_complete_store(int tag)
{
  for(int i = 0; i < this->entries.size(); i++){
    cout<<"input tag " <<tag <<" "<<this->entries[i].tag <<endl;
    if(this->entries[i].tag == tag && this->entries[i].opcode == "STORE"){
      cout << "Setting exec to true for instruction "<< this->entries[i].instruction_number <<endl;
      this->entries[i].exec = true;
    }
  }
}	

int ROB::get_size()
{
  return this->entries.size();
}

void ROB::print()
{
  cout<<"ROB :"<<endl;
  deque<ROB_Entry>::iterator iter = this->entries.begin();
  while(iter != this->entries.end()){
    ROB_Entry re = *iter;
    re.print();
    iter++;
  }
  cout<<endl;
}

