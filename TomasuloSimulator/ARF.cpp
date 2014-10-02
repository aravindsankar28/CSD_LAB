#include "ARF.h"


ARF::ARF(int size, bool debug)
{
  this->size = size;
  this->entries = new ARF_Entry[size];
  for (int i = 0; i < size; ++i)
  {
  	this->entries[i].index = i;
  	this->entries[i].tag = -1;
  }
  this->debug = debug;
}
ARF_Entry* ARF::get_entry(int index)
{
	return &(this->entries[index]);
}

void ARF::display()
{ 
  cout << "Reg" <<"\t"<<"Data"<<"\t" <<"Busy" <<"\t"<<"Tag"<<endl;
  for (int i = 0; i < this->size; ++i)
  {
    cout << this->entries[i].index << "\t" << this->entries[i].data << "\t" << this->entries[i].busy << "\t" 
	     << this->entries[i].tag <<endl;
  }
}