#include "LRU_Cache.h"
#include <cstdlib>

LRU_Cache::LRU_Cache(int size, int assoc, int blk_size, int hit_latency): Cache(size, assoc, blk_size, hit_latency, hit_latency)
{
  //set curr_access to 0
  curr_access = 0;
  
  //initialise last_use_matrix
  this->last_use_matrix = (int**)malloc(num_sets * sizeof(int*));
  for(int i = 0; i < num_sets; i++){
    this->last_use_matrix[i] = (int*) malloc(assoc * sizeof(int));
  }
  
  //initialise all entries in last_use_matrix to -1 (not used at all so far)
  for(int i = 0; i < num_sets; i++){
    for(int j = 0; j < assoc; j++){
      this->last_use_matrix[i][j] = -1;
    }
  }
  
}

void LRU_Cache::evict(int set)
{
  //Evict using LRU policy
  //TODO: Update curr_block, curr_set to the victimized block, set
  int victim = 0;

  for(int i = 1; i < assoc; i++){
    if(last_use_matrix[set][i] < last_use_matrix[set][victim]){
      victim = i;
    }
  }
  
<<<<<<< HEAD
  curr_set = set;
  curr_block = victim;
=======
  invalidate(block, victim);
  
  curr_block = block;
  curr_set = victim;
>>>>>>> ce142f6eb29b3eac9718cf8bca27f89a0f9f4afd
  
  return;
}

void LRU_Cache::read(uint64_t address)
{
  curr_access++;
  Cache::read(address);
  
  //update last_use_matrix based on current status
  if(!hit)
    last_use_matrix[curr_set][curr_block] = curr_access;
}

void LRU_Cache::write(uint64_t address)
{
  curr_access++;
  Cache::write(address);
  last_use_matrix[curr_set][curr_block] = curr_access; // set value as prev. curr_access +1, so as to maintain recency.
}
