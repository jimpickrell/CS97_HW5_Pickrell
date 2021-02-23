#include <string.h>


#ifndef OPTIONS
#define OPTIONS

#define CREATE 1
#define DESTROY 0



struct my_options {
    int nana;
    char *input_file;
    int input_option;
    long long nbytes;
    long long nbytes_per_line;
    int verbose_output;  // false
};

struct my_options * get_options(struct my_options * s, int argc, char ** argvg, int action);

#endif