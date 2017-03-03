make: arguments.c file_utils.c connectFour.c
		gcc -std=c99 -c arguments.c
		gcc -std=c99 -c file_utils.c
		gcc -std=c99 connectFour.c arguments.o file_utils.o
