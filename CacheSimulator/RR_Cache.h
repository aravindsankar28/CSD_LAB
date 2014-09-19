/* LRU_Cache.h
 * Class to model LRU_Cache
 */

#include "Cache.h"
//#include <cstdint>
#include <stdint.h>

#ifndef RR_CACHE_H
#define RR_CACHE_H
class RR_Cache : public Cache{
  //current access
  int curr_access;
  
  //stores when the block was last used
  //needed for book_keeping purposes
  //int **last_use_matrix;
  
  //Overriding base class method

  
public:
  void evict(int set);
  RR_Cache(int size, int assoc, int blk_size, int hit_latency);
  bool read(uint64_t address);
  void load(uint64_t address);
  
};

#endif
