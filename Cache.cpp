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
  this->misses = 0;
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

/**
 * If search succeeds, curr_block and curr_set is set to the value of the block.
 * hit is set to reflect success/failure of this search.
 */
bool Cache::search(int set, uint64_t tag)
{
  for(int i = 0; i < assoc; i++){
    if((this->addrs_stored[set][i])>>2 == tag){

      curr_set = set;
      curr_block = i;
      return true;
    }
  }

  //search has failed. Note that curr_block, curr_set will be unaffected if search fails
  hit = false;
  return false;
}

// Implementation wise, this is called ONLY when search fails.
void Cache::load(uint64_t address)
{
  accesses ++;
  int set = find_set(address);
  int i = 0;
  bool hit1 = false;
  for (i = 0; i <  assoc; ++i)
  {
    if(!is_valid(set,i))
    {
      //Empty block found. Set hit1 to reflect this.
      hit1 = true;
      break;
    }
  }
  // If hit1 is true, we found an empty block in that set 

  if (!hit1)
  {
    //Free space was not found. Evict a block.
    //Note: evict() only 'invalidates' the block and sets curr_set, curr_block to the 
    //location of the evicted block. If block is dirty, corresponding block(s) in lower level 
    //are made dirty.
    evict(set);
    i = curr_block;
  }

  //Store tag in the matrix.
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
    {
    	//cout <<"hi dirty" <<endl;
    	return true;
    }
  else
    {
    	//cout <<"hi not dirty" <<endl;
    	return false;
    }
}

void Cache::evict(int set)
{
//cout << "Evict " <<endl;
// Here, we need to do forceful eviction of blocks the upper levels.
  // To evict cur_set and cur_block 
  uint64_t address1 = (((addrs_stored[curr_set][curr_block] >>2) << map_bits) + set) <<offset_bits ;
  uint64_t address2 = address1 + (1 << offset_bits);
  uint64_t i = address1;
  //cout << "Start while at " <<size <<endl;
  while(i < address2 && upper_level != NULL)
  {
  	upper_level->accesses ++;
    if(upper_level->search(upper_level->find_set(i),upper_level->find_tag(i)))
    {
      upper_level->searchAndEvict(upper_level->find_set(i),upper_level->find_tag(i));
    }
    i += upper_level->blk_size;
  }
  //cout << "End while" <<size <<endl;
  // Forceful eviction done.


  //if block to be evicted is dirty, make it dirty at lower level (if we have a lower level)
  if(is_dirty(set, curr_block))
  {

    i = address1;
  
    while(i < address2 && lower_level != NULL)
    {
      lower_level->accesses ++;
      uint64_t lower_set = lower_level->find_set(i);
      //cout <<lower_level->search(lower_set, lower_level->find_tag(i))<<endl;
      int lower_blk = lower_level->curr_block;
      lower_level->make_dirty(lower_set, lower_blk);
      i += lower_level->blk_size;
    }
  }
  //cout << "Evict done" <<endl;
  return;
}

bool Cache::read(uint64_t address)
{
    accesses ++;
    bool result = search(find_set(address),find_tag(address));
    hit = result;
    if(result)
     {
     	hits++;
     } 
     else misses ++;
     
    return result;
}

void Cache::searchAndEvict(int set, uint64_t tag)
{
  //cout << " Search and Evict here" <<endl;
  uint64_t address1 = ((tag << map_bits) + set) << offset_bits ;
  uint64_t address2 = address1 + (1 << offset_bits);
  uint64_t i = address1;
  
  while(i < address2 && upper_level != NULL)
  {
    if(upper_level->search(upper_level->find_set(i),upper_level->find_tag(i)))
    {
      // block present in upper level
      upper_level->evict(upper_level->find_set(i));
    }
    i += upper_level->blk_size;
  }
  //cout << "Search and Evict done" <<endl;
}



void Cache::write(uint64_t address)
{

}
