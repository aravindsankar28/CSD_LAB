#include "Cache.h"
#include "CacheController.h"
#include <cstdio>

//dummy function
void hello()
{
  printf("hello\n");
}


void handle_read(Cache* caches, int n, uint64_t address)
{
  int i;
  bool hit = false;
  for (i = 0; i < n; ++i)
  {
    int set = caches[i].find_set(address);
    int tag = caches[i].find_tag(address);

    if(caches[i].search(set,tag) == true)
    {
      hit = true;
    break;
    }
  }
  if(hit)
  {
    for (int j = 0; j < i; ++j) // misses at all these levels 
    {
      caches[j].write(address);
    }
  }
  else
  {
    // miss at all caches
    for (i = 0; i < n; ++i)
    {
      caches[i].write(address);
    }
  }
}

void handle_write(Cache* caches,int n,uint64_t address)
{
  int i ;
  bool hit = false;

  for (int i = 0; i < n; ++i)
  {
    int set = caches[i].find_set(address);
    int tag = caches[i].find_tag(address);

    if(caches[i].search(set,tag) == true)
    {
      hit = true;
    break;
    }
  }

  if(hit)
  {
    for (int j = 0; j < i; ++j)
    {
      caches[j].write(address);
    }
  }
  else{

    // miss at all caches
    for (i = 0; i < n; ++i)
    {
      caches[i].write(address);
    }
    }
}
