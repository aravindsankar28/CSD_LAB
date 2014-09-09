#include "CacheController.h"
#include "Cache.h"
#include <cstdio>
#include <iostream>

using namespace std;
int count = 0;

void handle_read(Cache** caches, int n, uint64_t address)
{
  int i;
  bool hit = false;
  // Search in each cache starting from L1
  for (i = 0; i < n; ++i)
  {
    uint64_t set = caches[i]->find_set(address);
    uint64_t tag = caches[i]->find_tag(address);

    if(caches[i]->read(address)) // has to be read, we need to call the function that is overridden.
    {
      hit = true;
      break;
    }
  }

  count ++;
  if(hit)
  {
    // This means hit at level i and misses at all levels j < i
    for (int j = 0; j < i; ++j) // misses at all these levels 
    {
      // load address in to each of these caches
      caches[j]->load(address);
    }
  }
  else
  {
    // miss at all caches
    for (i = 0; i < n; ++i)
    {
      caches[i]->load(address);
    }
  }
}


void handle_write(Cache** caches, int n, uint64_t address)
{
  int i;
  bool hit = false;
  for (i = 0; i < n; ++i)
  {
    uint64_t set = caches[i]->find_set(address);
    uint64_t tag = caches[i]->find_tag(address);

    if(caches[i]->read(address)) // has to be read, we need to call the function that is overridden.
    {
      //WRITE HIT here.
      hit = true;
      break;
    }
  }
 
  count ++;
  
  //WRITE HIT : Load block into all levels above i.
  if(hit)
  {
    
    for (int j = 0; j < i; ++j) // misses at all these levels 
    {
      caches[j]->load(address);
    }
  }
  else
  {
    // WRITE MISS everywhere.
    for (i = 0; i < n; ++i)
    {
      caches[i]->load(address);
    }
  }
  
  //Make L1 dirty coz only that has most recent value.
  int dirty_set = caches[0]->find_set(address);
  int dirty_tag = caches[0]->find_tag(address);
  caches[0]->search(dirty_set, dirty_tag);
  int dirty_block = caches[0]->curr_block;
  
  caches[0]->make_dirty(dirty_set, dirty_block);
}

  