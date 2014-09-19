#include "Cache.h"

#ifndef CACHE_CONTROLLER_H
#define CACHE_CONTROLLER_H

/**
 * Function to manage read requests from memory
 * @param caches  : Array of caches that is our cache hierarchy 
 * @param n       : Number of caching levels
 * @param address : Address to read from
 */
void handle_read(Cache** caches, int n, uint64_t address);


void handle_write(Cache** caches, int n, uint64_t address);

#endif
