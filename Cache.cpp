/* Cache.cpp
 * Class for Modelling cache
 */

#include <iostream>
#include <cstdlib>
//#include <cstdint>
#include <stdint.h>
#include <cmath>
#include "Cache.h"
using namespace std;
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

/*                           s+b+2|s+b+1           s+2|s+1         2|     1      |      0
 *  ---------------------------------------------------------------------------------------------
 * |       Tag                    |   Map             |  Offset     |    Dirty   |     Valid     |
 * |  (64 - b - s) bits           |'b' bits           | 's' bits    |     Bit    |      Bit      |
 *  ---------------------------------------------------------------------------------------------
 * <--------------------------------------- (64-s) bits ----------------------------------------->
 * 
 * b --> map_bits
 * s --> offset_bits
 * Valid bit --> 0 if present, 1 if not
 * DirtyBit
 *
 */

uint64_t Cache::find_tag(uint64_t address)
{
  return address >> (map_bits+offset_bits);
}

// int Cache::find_block(uint64_t address)
// {
//   // Required ?
//   int tmp = address << (64-map_bits-set_bits);
//   return tmp >> set_bits;
//   
// }

uint64_t Cache::find_set(uint64_t address)
{
  // Basically return the map bits portion.
  return (address >> offset_bits) % (1<<map_bits);
}	

bool Cache::search(int set, uint64_t tag)
{
  //accesses++;
  for(int i = 0; i < assoc; i++){
    if((this->addrs_stored[set][i])>>2 == tag){
    //hit = true;
	  //hits++;
      curr_set = set;
      curr_block = i;
      return true;
    }
  }
  hit = false;
  //Note: curr_block and curr_set will be set by evict() in derived class
  return false;
}

// Implementation wise, this is called ONLY when search fails.
void Cache::write(uint64_t address)
{
  //TODO Model write access first before coding it
  int set = find_set(address);
  int i;
  bool hit1 = false;
  for (i = 0; i <  assoc; ++i)
  {
    if(!is_valid(set,i))
    {
      hit1 = true;
      break;
    }
  }
  if (!hit1)
  {
    // No free space - evict 
    evict(set);
    i = curr_block;
  }
  
  //Store tag in the matrix.
  //Note : Doing this will automatically validate the block as 
  //	   all user-space addresses are 48 bits long
  
  addrs_stored[set][i] = find_tag(address)<<2;
  
  return;
}

void Cache::invalidate(int set, int block)
{
  uint64_t mask = 1;
  
  //set valid bit i.e 0th bit to 1 by ORing with mask
  this->addrs_stored[set][block] |= mask; 
  return;
}

bool Cache::is_valid(int set, int block)
{
  uint64_t stored_value = addrs_stored[set][block];
  int valid_bit = stored_value % 2;
  if(valid_bit == 0)
    return true;
  else
    return false;
}

void Cache::make_dirty(int set, int block)
{
  uint64_t mask = 1;
  mask = mask << 1;
  this->addrs_stored[set][block] |= mask;
  return;
}

bool Cache::is_dirty(int set, int block)
{
  uint64_t stored_value = addrs_stored[set][block];
  int valid_bit = (stored_value >> 1) % 2;
  if(valid_bit == 0)
    return true;
  else
    return false;
}

void Cache::evict(int set)
{
    // To evict cur_set and cur_block 
  uint64_t address1 = (((addrs_stored[curr_set][curr_block] >>2) << map_bits) + set) <<offset_bits ;
  uint64_t address2 = address1 + (1 << offset_bits);
/*  cout << map_bits <<endl;

  cout << "("<< size <<"," << (addrs_stored[curr_set][curr_block] >>2)<<","<<set<<")"<<endl;
 */ 
  //cout << address1 <<endl;
  //cout << address2 <<endl;
  
  uint64_t i = address1;
  while(i <= address2 && upper_level != NULL)
  {
    if(upper_level->search(upper_level->find_set(i),upper_level->find_tag(i)))
    {
      // block present in upper level
      upper_level->searchAndEvict(upper_level->find_set(i),upper_level->find_tag(i));
    }

    i += upper_level->blk_size;
  }
  return;
}
bool Cache::read(uint64_t address)
{
    accesses ++;
    bool result = search(find_set(address),find_tag(address));
    if(result) hits++;
    return result;
}

void Cache::searchAndEvict(int set, uint64_t tag)
{
  uint64_t address1 = ((tag << map_bits) + set) << offset_bits ;
  uint64_t address2 = address1 + (1 << offset_bits);
  uint64_t i = address1;
  
  while(i <= address2 && upper_level != NULL)
  {
    if(upper_level->search(upper_level->find_set(i),upper_level->find_tag(i)))
    {
      // block present in upper level
      upper_level->evict(upper_level->find_set(i));
    }
    i += upper_level->blk_size;
  }

}



