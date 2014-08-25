#include "Cache.h"

#ifndef CACHE_CONTROLLER_H
#define CACHE_CONTROLLER_H

void hello();

/**
 * Function to manage read requests from memory
 * @param caches  : Array of caches that is our cache hierarchy 
 * @param n       : Number of caching levels
 * @param address : Address to read from
 */
void handle_read(Cache* caches, int n,uint64_t address);

/**
 * Function to manage write requests from memory
 * @param caches  : Array of caches that is our cache hierarchy 
 * @param n       : Number of caching levels
 * @param address : Address to write to
 */
void handle_write(Cache* caches, int n,uint64_t address);

#endif