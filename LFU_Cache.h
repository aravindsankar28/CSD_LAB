/* LFU_Cache.h
 * Class to model LFU_Cache
 */

#ifndef LFU_CACHE_H
#define LFU_CACHE_H

#include "Cache.h"
#include <cstdint>

class LFU_Cache : public Cache{
  
  int **frequency_matrix;
  
  //Overriding base class method
  void evict(int set);
  
public:
  LFU_Cache(int size, int assoc, int blk_size, int hit_latency);
  void read(uint64_t address);
  void write(uint64_t address);
  
};

#endif
