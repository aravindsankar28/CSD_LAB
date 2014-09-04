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
  //if(count < 3)
    //cout <<address << " "<<hit << endl;
  count ++;
  if(hit)
  {

    for (int j = 0; j < i; ++j) // misses at all these levels 
    {
      caches[j]->write(address);
    }
  }
  else
  {
    // miss at all caches
    for (i = 0; i < n; ++i)
    {
      caches[i]->write(address);
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

      hit = true;
      break;
    }
  }
 
  count ++;
  if(hit)
  {
    
    for (int j = 0; j < i; ++j) // misses at all these levels 
    {
      caches[j]->write(address);
    }
  }
  else
  {
    // miss at all caches
    for (i = 0; i < n; ++i)
    {
      caches[i]->write(address);
    }
  }
}

