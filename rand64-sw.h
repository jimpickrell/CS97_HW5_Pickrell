#ifndef MYSOFTWARE
#define MYSOFTWARE

// ============== rand64-sw.h

/* Input stream containing random bytes.  */
FILE *urandstream;
void software_rand64_init (void);
void software_rand64_file_init (char * s);
unsigned long long software_rand64 (void);
void software_rand64_fini (void);

#endif