#include "ResStation.h"

Res_Station::Res_Station(int size)
{
  this->size = size;
  this->entries = new Res_Station_Entry[size];
}
