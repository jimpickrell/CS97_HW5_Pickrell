/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   -mrdrnd is x86 compile option described here:
   https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include "randall.h"
#include "cpu_check.h"
#include "options.h"

//================ randall.c

// Main program, which outputs N bytes of random data.  
int main (int argc, char **argv)
{
  struct my_options s;
  get_options(&s, argc, argv, CREATE);

  // Check arguments.  
  bool valid = false;
  long long nbytes_total; 
  long long nbytes_per_line;
  int index;
  int c;
  int n;
  int i;
  //char * buffer;
  /*
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);  // convert string to long long
      if (errno)
	      perror (argv[1]);
      else
	      valid = !*endptr && 0 <= nbytes;
    }
  */

  // test probably ignored
/*
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }
*/

    // confirm that this worked
    nbytes_total = s.nbytes;
    if(s.verbose_output)printf("randall: bytes to print =  %lld\n", nbytes_total);
  
    nbytes_per_line = s.nbytes_per_line;
    if(nbytes_per_line == 0) nbytes_per_line = s.nbytes;
    if(s.verbose_output)printf("randall: bytes to print per line =  %lld\n", nbytes_per_line);

    //if(s.verbose_output)
    // printf("randall: input file selected = %s\n", s.input_file);

    // If there's no work to do, we are done.  
    if (s.nbytes == 0)
    {
        if(s.verbose_output)printf("Nothing to do, no bytes to write.\n");
        return 0;   
    }
  

    // Now that we know we have work to do, use appropriate library.  
        
    void (*initialize) (void);
    unsigned long long (*rand64) (void);
    void (*finalize) (void);

    // follow user input
    if(strcmp("rdrand", s.input_file) == 0) {
        // hardware generated random numbers.
        // Fallback to software is not allowed by spec
        if (rdrand_supported ())
        {
            initialize = hardware_rand64_init;
            initialize();  // mus do this here
            rand64 = hardware_rand64;
            finalize = hardware_rand64_fini;
        }
        else
        {
            printf("Error: Hardware rdrand requested but not available.  Sorry.  Exiting.\n");
            exit(1);
        }
    }
    else if(strcmp("mrand48_r", s.input_file) == 0) {
        initialize = software_rand64_init;
        initialize();  // must do this here
        rand64 = software_rand64;
        finalize = software_rand64_fini;
    }
    else {
        // user wants to use a file
        if(s.verbose_output)printf("File input requested.\n");
        // This must be done here because 
        // I don't want to use a function pointer.
        software_rand64_file_init (s.input_file);
        rand64 = software_rand64;
        finalize = software_rand64_fini;
        //exit(0);
    }

    //initialize ();
    // This requires an argument if a file is specified.
    // So I decided to move it up above and call it directly
    // instead of using a function pointer.
    int wordsize = sizeof rand64 ();
    int output_errno = 0;



    // use nbytes and nybtes per line and wordsize
    // create a buffer and fill it with random values
    // then send it to be output
    
    int words_per_line = nbytes_per_line/wordsize;
    if(nbytes_per_line%wordsize)words_per_line++;  // extra word just in case
    int buffer_size = words_per_line * wordsize;
    char *str = (char *)malloc((size_t)buffer_size);
    int * buffer = (int *)str;
    int nwritten = 0;
    int this_line_length;
    int j=0;
    
    do {
        // create a buffer full of random numbers
    
        if(s.verbose_output)printf("Line %d:\n",j);
        j++;
    
        int i;
        for(i=0; i< words_per_line; i++) {
            buffer[i]=rand64();
        }

        // Calculate how many should actually be written
        this_line_length = nbytes_total - nwritten;
        if (this_line_length>nbytes_per_line) this_line_length = nbytes_per_line;

        for(i=0; i<this_line_length; i++) {
            putchar(str[i]);
        }
        nwritten = nwritten + this_line_length;
        if(s.verbose_output)printf("[%d bytes]\n", this_line_length);
    }
    while (nwritten < nbytes_total);

    //// end of rewrite
/*
    do {
        unsigned long long x = rand64 ();
        int outbytes = nbytes < wordsize ? nbytes : wordsize;
        if (!writebytes (x, outbytes))  {
            output_errno = errno;
            break;
        }
        nbytes -= outbytes;
    }
    while (0 < nbytes);
*/

    if (fclose (stdout) != 0)
        output_errno = errno;
    if (output_errno)  {
        errno = output_errno;
        perror ("output");
    }

    finalize ();  // close files or devices
    free(str); // output buffer needs to be freed
    get_options(&s, argc, argv, DESTROY);  // free options object

    return !!output_errno;
}
