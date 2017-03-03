/*
 * Created by Miles De Wind - 02/22/2017
 * Utilized code provided by Ira Woodring
 */

#include <stdlib.h>
#include "arguments.h"

// Program title and version
const char *argp_program_version = "demonstrate_args 1.0";
// Who to contact if bugs are found
const char *argp_program_bug_address = "woodriir@not_an_address.com";

// Documentation string.
char doc[] = "Demonstrate Args - a program to demonstrate how to use argp to parse command-line agruments.";

// Description of the required arguments we accept.
char args_doc[] = "";

// This option struct is for any optional arguments.
struct argp_option options[] = {
	{"width",	'w',	"WIDTH",	0,	"The width of the board - the horizontal number of cells, default 7. If called with both, the last call takes precidence" },
	{"height",	'h',	"HEIGHT",	0,	"Sets the height of the board - the vertical number of cells, default 7. If called with both, the last call takes precidence" },
	{"connect",	'c',	"CONNECT",	0,	"Sets the number of connected peices required to win, default 4." },
	{"both",	'b',	"BOTH",		0,	"Sets both the height and width of the board. If called with width or height, the last call takes precidence." },
	{"load",	'l',	"LOAD", 	0, 	"Load saved game state" },
	{ 0 }
};

// Argp requires us to pass these values in via the argp struct
struct argp argp = { options, parse_opt, args_doc, doc };
// Declare a variable called arguments that is also an
// arguments struct.  Remeber - we can't use the struct
// by just describing it.  We also need to make one.  Sort
// of similar to how we instantiate an object in Java.
struct arguments arguments;

// Our custom function for setting up values once they
// have been parsed from the command line args.
error_t parse_opt(int key, char* arg, struct argp_state *state){
	struct arguments *arguments = state->input;
	switch(key){
		case 'b':
			arguments->first = atoi(arg);
			arguments->second = atoi(arg);
			break;
		case 'w':
			arguments->first = atoi(arg);
			break;
		case 'h':
			arguments->second = atoi(arg);
			break;
		case 'c':
			arguments->third = atoi(arg);
			break;
		case 'l':
			arguments->load_file = arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

// Here we setup some default values and override them
// if we get new values from the commandline.
struct arguments setup(int argc, char** argv){
	// Default of first, second, and load_file
	int width = 7;
	int height = 7;
        int connect = 4;
	char* load_file = NULL;
	// Call argp to do its thing.
	argp_parse(&argp, argc, argv, 0, 0, &arguments);
	// Return values are in the arguments struct.  Pull
	// them out if they are there.
	if(arguments.first != 0){
	  width = arguments.first;
	}
	if(arguments.second != 0){
	  height = arguments.second;
	}
	if(arguments.third != 0){
    connect = arguments.third;
	}
        if(connect > width || connect > height){
	  width = -1;
	  height = -1;
	  connect = -1;
	}
	printf("width: %d\nHeight: %d\nConnect: %d\n\n", width, height, connect);
	if(arguments.load_file != NULL){
		printf("load file: %s\n\n", arguments.load_file);
	}
	arguments.first = width;
	arguments.second = height;
	arguments.third = connect;
	return arguments;
}
