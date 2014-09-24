#include <string>
#include "ResStation.h"
#include <iostream>

using namespace std;

 Res_Station_Entry::Res_Station_Entry()
  {
    this->src1_tag =-1;
    this->src2_tag = -1;
    this->src1_data_present = 0;
  	this->src2_data_present = 0;
  }


Res_Station::Res_Station(int max_size)
{
  this->max_size = max_size;
  this->entries = new Res_Station_Entry[max_size];
  this->size = 0;
  
}

void Res_Station::add_entry(Res_Station_Entry res_station_entry)
{
	for (int i = 0; i < max_size; ++i)
	{
		if(! this->entries[i].busy)
		{
			this->entries[i] = res_station_entry;
			size ++;
			//cout << res_station_entry.src2_data_present <<endl;
			return;
		}
	}
}
