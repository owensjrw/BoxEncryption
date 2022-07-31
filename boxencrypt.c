#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXROWS 9
#define MAXCOLS 6

int *getRowCol(char c, const char key[MAXROWS][MAXCOLS]){
  static int rowCol[2];
  for(int i = 0; i < MAXROWS; i++){
    for(int j= 0; j < MAXCOLS; j++){
      if(c == key[i][j]){
        rowCol[0] = i;
        rowCol[1] = j;
        return rowCol;
      }
    }
  }
  return NULL;
}

void encryptPair(char first, char second, char *rfirst, char *rsecond,
                 const char key[MAXROWS][MAXCOLS]){
  int rowColFirst[2];
  int rowColSecond[2];
  int *rowCol;
  rowCol = getRowCol(first, key);
  if(rowCol == NULL){
    rowColFirst[0] = MAXROWS;
    rowColFirst[1] = MAXCOLS;
  } else {
    rowColFirst[0] = rowCol[0];
    rowColFirst[1] = rowCol[1];
  }
  rowCol = getRowCol(second, key);
  if(rowCol == NULL){
    rowColSecond[0] = MAXROWS;
    rowColSecond[1] = MAXCOLS;
  } else {
    rowColSecond[0] = rowCol[0];
    rowColSecond[1] = rowCol[1];
  }

  if( (rowColFirst[0] == rowColSecond[0])                              ||
      (rowColFirst[1] == rowColSecond[1])                              ||
      ((rowColFirst[0] == MAXROWS) && (rowColFirst[1] == MAXCOLS))     ||
      ((rowColSecond[0] == MAXROWS) && (rowColSecond[1] == MAXCOLS))   ){
    *rfirst = second;
    *rsecond = first;
  } else {
    *rfirst = key[rowColFirst[0]][rowColSecond[1]];
    *rsecond = key[rowColSecond[0]][rowColFirst[1]];
  }

}

int main(void){
  const char key[MAXROWS][MAXCOLS] = { {'Q', 'A', 'Z','W', 'S', 'X'},
                                       {'E', 'D', 'C','R', 'F', 'V'},
                                       {'T', 'G', 'B','Y', 'H', 'N'},
                                       {'U', 'J', 'M','I', 'K', 'O'},
                                       {'L', 'P', 'q','a', 'z', 'w'},
                                       {'s', 'x', 'e','d', 'c', 'r'},
                                       {'f', 'v', 't','g', 'b', 'y'},
                                       {'h', 'n', 'u','j', 'm', 'i'},
                                       {'k', 'o', 'l','p', '1', ' '} };
   char *toEncrypt;
   size_t getline_n = 0;
   size_t charcount;
   puts("Enter a string to be encyrpted:");
   charcount = getline(&toEncrypt, &getline_n, stdin);
   if(charcount == -1){
     perror("Error: Unalbe to get input.");
     exit(EXIT_FAILURE);
   }
   char *encryptMessage;
   int toEncryptLength = strlen(toEncrypt);
   toEncrypt[toEncryptLength - 1] = '\0';
   toEncryptLength = strlen(toEncrypt);
   encryptMessage = malloc(toEncryptLength + 1);

   for(int i = 0; i < (toEncryptLength - 1); i += 2){
     encryptPair(toEncrypt[i], toEncrypt[i + 1], &encryptMessage[i],
                 &encryptMessage[i + 1], key);
   }

   if(toEncryptLength % 2 == 0){
     encryptMessage[toEncryptLength] = '\0';
   } else {
     encryptMessage[toEncryptLength - 1] = toEncrypt[toEncryptLength - 1];
     encryptMessage[toEncryptLength] = '\0';
   }

   printf("Original Text: %s\n", toEncrypt);
   printf("Cypher Text: %s\n", encryptMessage);

   free(toEncrypt);
   free(encryptMessage);
   return EXIT_SUCCESS;
}
