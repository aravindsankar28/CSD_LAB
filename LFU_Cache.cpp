#include "LFU_Cache.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;
LFU_Cache::LFU_Cache(int size, int assoc, int blk_size, int hit_latency): Cache(size, assoc, blk_size, hit_latency, hit_latency)
{
  //initialise frequency_matrix
  this->frequency_matrix = (int**)malloc(num_sets * sizeof(int*));
  for(int i = 0; i < num_sets; i++){
    this->frequency_matrix[i] = (int*) malloc(assoc * sizeof(int));
  }
  
  //initialise all entries in last_use_matrix to -1 (not used at all so far)
  for(int i = 0; i < num_sets; i++){
    for(int j = 0; j < assoc; j++){
      this->frequency_matrix[i][j] = 0;
    }
  }
}

void LFU_Cache::evict(int set)
{
  int victim = 0;
  
  for(int i = 0; i < assoc; i++){
    if(frequency_matrix[set][i] < victim)
      victim = i;
  }
  
  invalidate(set, victim);
  
  curr_block = victim;
  curr_set = set;
  Cache::evict(set);
}

bool LFU_Cache::read(uint64_t address)
{
 // cout << "hi read" <<endl;
  bool result = Cache::read(address);
 
  if(hit)
    frequency_matrix[curr_set][curr_block]++;
  else
    frequency_matrix[curr_set][curr_block] = 0;
  return result;
}

void LFU_Cache::write(uint64_t address)
{
  Cache::write(address);
  if(hit)
    frequency_matrix[curr_set][curr_block]++;
  else
    frequency_matrix[curr_set][curr_block] = 0;
}
