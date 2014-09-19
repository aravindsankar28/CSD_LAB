#ifndef RES_STATION_H
#define RES_STATION_H

/**
 * One entry in the reservation station
 */
struct Res_Station_Entry{
  // opcode of instruction that has been seen
  int opcode;
  
  //RRF index corresponding to required operands
  int src1;
  int src2;
  
  //RRF index corresponding to destination
  int dest;
  
  //Finished execution or not?
  bool exec_over;
  
  //TODO: Do we need valid and busy bits here?
};

class Res_Station{
  int size;
  Res_Station_Entry *entries;
public:
  Res_Station(int size);
};