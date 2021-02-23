#include "options.h"
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct my_options * get_options(struct my_options *s, int argc, char** argv, int action)
{
    //struct my_options t;
    // Check arguments.  
    
    bool valid = false;
    long long nbytes;
    long long nbytes_per_line;
    int index;
    int c;
    int n;
    int i;
    char * buffer;

    if(action == DESTROY) {
        // destroy and free options
        if(s->verbose_output)printf("my_options: Destroy options.\n");
        if(s->input_file) {
            if(s->verbose_output)printf("Freeing input file text");
            free(s->input_file);
        }
        return s;
    }
    else 
    {
        // create options with default values
        if(s->verbose_output)printf("my_options: Create options argc = %d\n", argc);
        s->input_file=strdup("rdrand");  // -i default value
        s->nbytes=0;                     // positional argument default value
        s->nbytes_per_line=0;            // -o default value
        s->verbose_output=0;             // false
    }

    while ((c = getopt(argc, argv, "vi:o:")) != -1) {

        if(s->verbose_output)printf("User selected option %c\n",c);

        switch(c) {
            case 'i':
                if(s->verbose_output)printf("Option -i (input) was selected with argument %s\n", optarg);

                if(strcmp(optarg, "rdrand")==0) {
                    if(s->verbose_output)printf("Option i chosen with input=rdrand.\n");
                    s->input_option = 1;
                    //s->input_file = 0;
                    if(s->input_file)free(s->input_file);
                    s->input_file = strdup(optarg);
                }
                else if(strcmp(optarg, "mrand48_r")==0) {
                    if(s->verbose_output)printf("Option i chosen with input=mrand48_r.\n");
                    s->input_option = 2;
                    //s->input_file = 0;
                    if(s->input_file)free(s->input_file);
                    s->input_file = strdup(optarg);
                }
                else {
                    if(s->verbose_output)printf("Option i chosen, so we will input from file=%s.\n", optarg);
                    s->input_option = 3;
                    if(s->input_file)free(s->input_file);
                    s->input_file = strdup(optarg);
                    if(s->verbose_output)printf("Option i confirming input from file=%s.\n", s->input_file);
                    
                    // actually we should start with /
                    // t.input_file = strdup(optarg[1])

                    // Check that the first character is a slash

                    if(s->input_file[0] == '/') {
                        if(s->verbose_output)printf("First character of file option is a slash.\n");
                    }
                    else {
                        printf("Error: First character of file option %s is not a slash.\n",s->input_file);
                    }
                }

                valid=true;

            break;

            //option '-i input', where input is one of the following:
            // rdrand – the hardware random-number generation supported by x86-64 processors if available; 
            // randall should issue an error message and fail if it is not available. This option is the default.
            // mrand48_r – the mrand48_r function of the GNU C library.
            // /F (i.e., any argument beginning with /) – use the file /F as a source of random data, instead 
            // of using /dev/random.

            case 'o':
               
                if(s->verbose_output)printf("Option -o (output) selected with argument %s\n", optarg);

                if(strcmp(optarg, "stdio")==0) {
                    if(s->verbose_output)printf("Option chosen to send output to standard output.\n");
                }
                else {
                    n = atoi(optarg);
                    nbytes_per_line = atoll(optarg);
                    if(n==0){
                        printf("Bad number error. Aborting.\n");
                        exit(EXIT_FAILURE);
                    }               
                }
                if(s->verbose_output)printf("Option chosen bytes to be sent in bytes per line = %lld.\n", nbytes_per_line);

                // buffer = (char*) malloc (n);
                //if (buffer==NULL) exit (1);

                //for (i=0; i<n; i++)
                //  buffer[i]=rand()%26+'a';
                //buffer[i]='\0';

                // printf ("Random string: %s\n",buffer);
                //free (buffer);

                s->nbytes_per_line = nbytes_per_line;

                valid=true;
                //}


                // option -o N
                // stdio – Use stdio output, as before. This is the default.
                // N (a positive decimal integer) – Output N bytes at a time, using the write system call. 
                // If the write call reports a successful write of fewer than N bytes, do not consider 
                // this to be an error; just add the successfully-written number of bytes to your count 
                // of bytes written. The last output block might be smaller than usual, if needed to get the 
                // total size to be correct. You can use the malloc function to allocate your output buffer.

            break;

            case 'v':
                printf("Verbose output option selected.\n");
                s->verbose_output = 1;
                break;

            default:
                printf("Error: Unknown option %c with argument %s\n", c, optarg);
                valid = false;
            break;

        }
    }

    for (index = optind; index < argc; index++) {
        if(s->verbose_output)printf ("Positional argument: number of bytes to generate = %s\n", argv[index]);
        n = atoi(argv[index]);
        nbytes = atoll(argv[index]);
        s->nbytes=nbytes;        
        if(s->verbose_output)printf ("Random bytes int %d\n", n);
        valid=true;
    }

    s->nbytes = nbytes;
    return s;
}