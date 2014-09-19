#include "LRU_Cache.h"
#include <cstdlib>
#include <iostream>

using namespace std;

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
  
  invalidate(set, victim);
  
  curr_block = victim;
  curr_set = set;
  Cache::evict(set);
  return;
}

bool LRU_Cache::read(uint64_t address)
{
  
  curr_access++;
  bool result = Cache::read(address);
  // curr_block and curr_set would've been set 
  //update last_use_matrix based on current status
  if(!hit)
    last_use_matrix[curr_set][curr_block] = curr_access;
  return result;
}

void LRU_Cache::load(uint64_t address)
{
  curr_access++;
  Cache::load(address);
  last_use_matrix[curr_set][curr_block] = curr_access; // set value as prev. curr_access +1, so as to maintain recency.
}
