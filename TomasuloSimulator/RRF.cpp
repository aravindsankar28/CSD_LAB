#include "RRF.h"

RRF::RRF(int size)
{
  this->size = size;
  this->entries = new RRF_Entry[size];
}