#include "ROB.h"
ROB_Entry::ROB_Entry(int tag,string opcode)
{
	this->tag = tag;
	this->opcode = opcode;
	this->exec = false;

}

ROB_Entry::ROB_Entry()
{
	this->tag = -1;
	this->exec = false;
	this->opcode = "";
	this->debug = false;

}

ROB::ROB(int max_size, bool debug)
{
	this->max_size = max_size;
	entries = *(new deque<ROB_Entry>());
	scratch_queue = *(new deque<ROB_Entry>());
	scratch_queue.clear();
	this->debug = debug;

}

void ROB_Entry::print()
{ 
	cout << "Tag :"<<this->tag<<" : "<< "IP :"<<this->instruction_number<<" : "<< "Exec :"<<this->exec << endl;
}


bool ROB::attempt_push(ROB_Entry r)
{
	if (this->entries.size() < max_size){
	    if(this->debug)  
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
		    if(this->debug)  
			cout << "Popping from rob to commit and retire instruction "<<re.instruction_number <<endl;
		    this->scratch_queue.push_back(re);
		    this->entries.pop_front();  
		    if(this->entries.size() == 0)
			break;
		    re = this->entries.front();
		}
		return true;
	}
	else
		return false;
}

void ROB::set_complete(int tag)
{

	for(int i = 0; i < this->entries.size(); i++){

		if(this->entries[i].tag == tag &&  this->entries[i].opcode != "STORE"){
		    if(this->debug)  
			cout << "Setting exec to true for instruction "<< this->entries[i].instruction_number <<endl;
		    this->entries[i].exec = true;
		}
	}

}

void ROB::set_complete_store(int tag)
{
	for(int i = 0; i < this->entries.size(); i++){
		if(this->entries[i].tag == tag && this->entries[i].opcode == "STORE"){
		if(this->debug)  
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
if(this->debug)  {
	cout<<"ROB :"<<endl;
 
	deque<ROB_Entry>::iterator iter = this->entries.begin();
	while(iter != this->entries.end()){
		ROB_Entry re = *iter;
		re.print();
		iter++;
	}
	cout<<endl;
  }
}

