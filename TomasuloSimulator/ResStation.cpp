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
  	this->src1_data = -1;
  	this->src2_data = -1;
  }


Res_Station::Res_Station(int max_size)
{
  this->max_size = max_size;
  this->entries = new Res_Station_Entry[max_size];
  this->size = 0;
  for (int i = 0; i < max_size; ++i)
  {
  	this->entries[i].index = i;
  }
}

Res_Station_Entry* Res_Station::get_free_entry()
{
	for (int i = 0; i < max_size; ++i)
	{
		if(! this->entries[i].busy)
			return &this->entries[i];
		
	}
}
void Res_Station::display()
{
	cout << "RESERVATION STATION :"<<endl;
	cout << "opcode" <<"\t"<<"dest_t"<<"\t"<<"src1" <<"\t"<<"src1_p"<< "\t"<<"src2"<<"\t"<<"src2_p"<<"\t"<<endl;
	for (int i = 0; i < max_size; ++i)
	{
		if(this->entries[i].busy)
		{
			this->entries[i].display();
			//cout << entries[i].opcode <<"\t"<<entries[i].dest_tag << "\t" <<entries[i].src1_data <<"\t"<<entries[i].src2_data <<endl;
		}
	}
}
void Res_Station_Entry::display()
{
	cout << opcode <<"\t("<<dest_tag << ")\t" <<src1_data <<"\t"<<src1_data_present<<"\t"<<src2_data <<"\t"<<src2_data_present<<"\t"<< endl;
}
Res_Station_Entry* Res_Station::get_entry(int index)
{
	return &entries[index];
}

void Res_Station::remove_entry(Res_Station_Entry* res_station_entry)
{
	this->entries[res_station_entry->index].busy = false;
	this->entries[res_station_entry->index].src1_data_present = 0;
	this->entries[res_station_entry->index].src2_data_present = 0;
	this->size --;	
}