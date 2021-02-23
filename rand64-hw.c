// ============== rand64-hw.c
#include <immintrin.h>
#include "rand64-hw.h"

/* Initialize the hardware rand64 implementation.  */
//static 
void hardware_rand64_init (void)
{
}

// Return a random value, using hardware operations.  
//static 
unsigned long long hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

/* Finalize the hardware rand64 implementation.  */
//static 
void hardware_rand64_fini (void)
{
}