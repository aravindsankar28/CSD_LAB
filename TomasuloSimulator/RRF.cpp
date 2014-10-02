#include "RRF.h"
RRF::RRF(int size, bool debug)
{
  this->size = size;
  this->entries = new RRF_Entry[size];
  for (int i = 0; i < size; ++i)
  {
  	this->entries[i].index = i;
  }
  this->debug = debug;
}

int RRF::find_non_busy_register()
{
	for (int i = 0; i < size; ++i)
	{
		if(this->entries[i].busy == 0)
			return i;
	}
	return -1;
}
RRF_Entry* RRF::get_entry(int index)
{
	return &(this->entries[index]);
}

void RRF::display()
{
    if(this->debug)  {
	cout << "Reg"<<"\t"<<"Data"<<"\t" <<"Busy" <<"\t"<<"Valid"<<endl;
	for (int i = 0; i < this->size; ++i)
	{
	  cout << this->entries[i].index << "\t" << this->entries[i].data << "\t" << this->entries[i].busy << "\t" 
	  << this->entries[i].valid <<endl;
	}
    }
 
}