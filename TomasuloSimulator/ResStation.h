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

  int instruction_number;

  //RRF index corresponding to required operands
  int src1_tag;
  int src2_tag;
  
  int src1_data;
  int src2_data;

  int src1_data_present;
  int src2_data_present;

  //RRF index corresponding to destination
  int dest_tag;
  
  //Finished execution or not?
  bool exec_over;

  bool busy;  
  //TODO: Do we need valid and busy bits here?

  int index;

  void display();

 Res_Station_Entry();
};

class Res_Station{
  

public:
  Res_Station_Entry *entries;
  int size;
  int max_size;
  Res_Station(int max_size, bool debug);
  Res_Station_Entry* get_free_entry();
  Res_Station_Entry* get_entry(int index);
  void display();
  void remove_entry(Res_Station_Entry* res_station_entry);
  bool debug;
};

#endif
 