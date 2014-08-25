/* Cache.cpp
 * Class for Modelling cache
 */

#include <iostream>
#include <cstdlib>
//#include <cstdint>
#include <stdint.h>
#include <cmath>
#include "Cache.h"

Cache::Cache(int size, int assoc, int blk_size, int hit_latency, int policy){
  this->size = size;
  this->assoc = assoc;
  this->blk_size = blk_size;
  this->hit_latency = hit_latency;
  this->policy = policy; 
   
  //find num_sets
  this->num_sets = (size*1024) / (blk_size*assoc); // Number of sets 
  this->map_bits = ceil(log2(num_sets)); // Number of middle bits (map bits).
  this->set_bits = ceil(log2(assoc)); // Number of bits to index inside each set.
  this->offset_bits = ceil(log2(blk_size)); // Number of bits for offset 

  //initialise addrs_stored matrix
  this->addrs_stored = (uint64_t**)malloc(num_sets * sizeof(uint64_t*));
  for(int i = 0; i < num_sets; i++){
    this->addrs_stored[i] = (uint64_t*) malloc(assoc * sizeof(uint64_t));
  }
  
  //invalidate all cache blocks initially
  for(int i = 0; i < num_sets; i++){
    for(int j = 0; j < assoc; j++){
      this->invalidate(i, j);
    }
  }
    
  //initialise stats
  this->accesses = 0;
  this->hits = 0;
  this->miss_ratio = 0;
 }

/*           63|62                        s+b|s+b-1             s|s-1          0
 *  ---------------------------------------------------------------------------
 * | Valid bit | Tag                         |Map                | Offset      |
 * |  1 bit    | (64 - b - s) bits           |'b' bits           | 's' bits    |
 *  ---------------------------------------------------------------------------
 * <----------------------------- (64-s) bits --------------------------------->
 * 
 * b --> map_bits
 * s --> offset_bits
 * Valid bit --> 0 if present, 1 if not
 *
 */

int Cache::find_block(uint64_t address)
{
	// Required ?
  int tmp = address << (64-map_bits-set_bits);
  return tmp >> set_bits;

}

int Cache::find_set(uint64_t address)
{
	// Basically return the map bits portion.
  return (address >> offset_bits) % (1<<map_bits);
  //return address << (64-map_bits);
}	

bool Cache::search(int set, uint64_t tag)
{
  for(int i = 0; i < assoc; i++){
    if(this->addrs_stored[set][i] == tag){
      hit = true;
      curr_set = set;
      curr_block = i;
      return true;
    }
  }
  hit = false;
  
  //Note: curr_block and curr_set will be set by evict() in derived class
  return false;
}

void Cache::read(uint64_t address)
{
  //TODO Model read access first before coding it
  hit = false;
  curr_block = -1;
  curr_set = -1;
}

void Cache::write(uint64_t address)
{
  //TODO Model write access first before coding it
  hit = false;
  curr_block = -1;
  curr_set = -1;
}

void Cache::invalidate(int set, int block)
{
  uint64_t mask = 1;
  mask = mask << 63;
  this->addrs_stored[set][block] |= mask; // sets it 1 - so as to invalidate
  return;
}

bool Cache::is_valid(int set, int block)
{
  uint64_t stored_value = addrs_stored[set][block];
  int valid_bit = stored_value >> 63;
  if(valid_bit == 0)
    return true;
  else
    return false;
}
void Cache::evict(int set)
{

}

  
