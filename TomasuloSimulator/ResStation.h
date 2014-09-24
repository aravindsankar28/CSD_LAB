#ifndef RES_STATION_H
#define RES_STATION_H

#include <stdint.h>
#include <string>
/**
 * One entry in the reservation station
 */
 using namespace std;

class Res_Station_Entry{
public:
  // opcode of instruction that has been seen
  string opcode;
  
  //RRF index corresponding to required operands
  int src1_tag;
  int src2_tag;
  
  uint64_t src1_data;
  uint64_t src2_data;

  int src1_data_present;
  int src2_data_present;

  //RRF index corresponding to destination
  int dest_tag;
  
  //Finished execution or not?
  bool exec_over;

  bool busy;  
  //TODO: Do we need valid and busy bits here?

 Res_Station_Entry();
};

class Res_Station{
  
  Res_Station_Entry *entries;
public:
  int size;
  int max_size;
  Res_Station(int max_size);
  void add_entry(Res_Station_Entry res_station_entry);
};

#endif