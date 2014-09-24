#include "RRF.h"

RRF::RRF(int size)
{
  this->size = size;
  this->entries = new RRF_Entry[size];
  for (int i = 0; i < size; ++i)
  {
  	this->entries[i].index = i;
  }
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
RRF_Entry RRF::get_entry(int index)
{
	return this->entries[index];
}