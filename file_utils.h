#ifndef FILE_UTILS_H
#define FILE_UTILS_H

//displays the gameboard
int display(int ** board, int width, int height);

//checks diagnal win conditions diagnally up
int checkDiagnalUp(int** board, int width, int height,
  int colNum, int rowNum, int connect, int player);

//checks win conditions diagnally down
int checkDiagnalDown(int** board, int width, int height,
  int colNum, int rowNum, int connect, int player);

//checks vertical win condiditons
int checkVert(int** board, int width, int colNum, int connect, int player);

//checks horizontal win conditions
int checkHor(int** board, int height, int rowNum, int connect, int player);

int read_file(char* filename, char **buffer);

int write_file(char* filename, char *buffer, int size);

int saveGame(int** board, int width, int height, int connect, int player,
   char* save);

int** loadGame(char* buffer, char* load);

//Changes lowest open position in column to player (the players number)
int ** playCol(int** board, int width, int height, int colNum, int player);

#endif
