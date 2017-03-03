make: arguments.c file_utils.c connectFour.c
		gcc -c arguments.c
		gcc -c file_utils.c
		gcc connectFour.c arguments.o file_utils.o
