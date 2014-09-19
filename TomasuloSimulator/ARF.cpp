#include "ARF.h"

ARF::ARF(int size)
{
  this->size = size;
  this->entries = new ARF_Entry[size];
}


