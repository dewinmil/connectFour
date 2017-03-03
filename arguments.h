#ifndef H_ARGUMENTS
#define H_ARGUMENTS

#include <argp.h>

// Define the arguments struct that will hold the values
// for flags that provide arguments.

struct arguments {
	int first;
	int second;
	int third;
	char *load_file;
	char *save_file;
};

// Function to setup values once they are parsed.
error_t parse_opt(int key, char* arg, struct argp_state *state);

// Function to begin the parsing process.
struct arguments setup(int argc, char** argv);

#endif
