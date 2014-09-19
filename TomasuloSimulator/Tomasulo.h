#ifndef TOMASULO_H
#define TOMASULO_H

#include <cstdint>

#include "RRF.h"
#include "ARF.h"
#include "ResStation.h"
#include "ROB.h"

/**
 * Modelled the registers as 64-bit integers
 */
typedef reg uint64_t;

/**
 * This must be equal to number of entries in ARF
 */
int num_arch_reg;

/**
 * This must be equal to number of entries in RRF
 */
int num_renamed_reg;

/**
 * This must be the number of entries in reservation station
 */
int num_rs_entries;

/**
 * Elements of our Tomasulo algorithm
 */
ARF arf;
RRF rrf;
Res_Station rs;
ROB rob;

/**
 * Get number of cycles reqd fr execution of opcode.
 */
int get_cycles(int opcode);


