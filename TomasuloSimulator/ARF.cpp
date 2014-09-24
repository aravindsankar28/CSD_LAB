#include "ARF.h"

ARF::ARF(int size)
{
  this->size = size;
  this->entries = new ARF_Entry[size];
  for (int i = 0; i < size; ++i)
  {
  	this->entries[i].index = i+1;
  }
}
ARF_Entry ARF::get_entry(int index)
{
	return entries[index];
}

