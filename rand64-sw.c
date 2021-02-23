// ============== rand64-sw.c
#include <stdio.h>
#include <stdlib.h>
#include "rand64-sw.h"

/* Software implementation.  */

/* Initialize the software rand64 implementation.  */
//static 
void software_rand64_init (void)
{
  urandstream = fopen ("/dev/random", "r");
  if (! urandstream)
    abort ();
}

// same but with a filename
//static 
void software_rand64_file_init (char * s)
{
  urandstream = fopen (s, "r");
  if (! urandstream)
    abort ();
}



/* Return a random value, using software operations.  */
//static 
unsigned long long software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
//static 
void software_rand64_fini (void)
{
  fclose (urandstream);
}