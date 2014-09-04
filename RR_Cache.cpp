#include "RR_Cache.h"
#include <cstdlib>

RR_Cache::RR_Cache(int size, int assoc, int blk_size, int hit_latency): Cache(size, assoc, blk_size, hit_latency, hit_latency)
{
  //set curr_access to 0
  curr_access = 0; 
}

void RR_Cache::evict(int set)
{
  //Evict using LRU policy
  //TODO: Update curr_block, curr_set to the victimized block, set
  int victim = 0;
  victim = rand()%assoc; // random block no. in (0,assoc)

/*  for(int i = 1; i < assoc; i++){
    if(last_use_matrix[set][i] < last_use_matrix[set][victim]){
      victim = i;
    }
  }
  */
  invalidate(set, victim);
  
  curr_block = victim;
  curr_set = set;
  
  return;
}

void RR_Cache::read(uint64_t address)
{
  curr_access++;
  Cache::read(address);
  
}

void RR_Cache::write(uint64_t address)
{
  curr_access++;
  Cache::write(address);
}
