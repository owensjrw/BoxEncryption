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
        rowCol[1] = j;
        break;
      }
    }
    if(c == key[i][rowCol[1]]){
      rowCol[0] = i;
      break;
    }
  }
  return rowCol;
}

void encryptPair(char first, char second, char *rfirst, char *rsecond, const char key[MAXROWS][MAXCOLS]){
  int rowColFirst[2];
  int rowColSecond[2];
  int *rowCol;
  rowCol = getRowCol(first, key);
  rowColFirst[0] = rowCol[0];
  rowColFirst[1] = rowCol[1];
  rowCol = getRowCol(second, key);
  rowColSecond[0] = rowCol[0];
  rowColSecond[1] = rowCol[1];

  if((rowColFirst[0] == rowColSecond[0]) || (rowColFirst[1] == rowColSecond[1])){
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
   const char *toEncrypt = "A very Long STring To Encrypt with a 1";
   char *encryptMessage;
   int toEncryptLength = strlen(toEncrypt);
   encryptMessage = malloc(sizeof encryptMessage * (toEncryptLength + 1));

   for(int i = 0; i < (toEncryptLength - (toEncryptLength % 2)); i += 2){
     encryptPair(toEncrypt[i], toEncrypt[i + 1], &encryptMessage[i], &encryptMessage[i + 1], key);
   }

   if(toEncryptLength % 2 == 0){
     encryptMessage[toEncryptLength] = '\0';
   } else {
     encryptMessage[toEncryptLength - 1] = toEncrypt[toEncryptLength - 1];
     encryptMessage[toEncryptLength] = '\0';
   }

   printf("Original Text: %s\n", toEncrypt);
   printf("Cypher Text: %s\n", encryptMessage);

   free(encryptMessage);
   return EXIT_SUCCESS;
}
