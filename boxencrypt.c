#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXROWS 8
#define MAXCOLS 8

int found(char m[][8], char val)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (m[r][c] == val)
                return 1;
        }
    }
    return 0;
}

void generate_key(char key[][8])
{
    int i;
    char temp[64] = { ' ' };
    int k = 0;
    temp[k++] = ' ';
    temp[k++] = '.';

    for (i = 0; i < 10; i++, k++)
        temp[k] = '0' + i;

    for (i = 0; i < 26; i++, k++)
        temp[k] = 'A' + i;

    for (i = 0; i < 26; i++, k++)
        temp[k] = 'a' + i;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {

            i = rand() % 64;
            while (found(key, temp[i]) == 1)
            {
                i = rand() % 64;
            }
            key[r][c] = temp[i];
        }
    }
}

//Function to get row and column from key
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

//Function to encrypt by pairs
void encryptPair(char first, char second, char *rfirst, char *rsecond,
                 const char key[MAXROWS][MAXCOLS]){
  int rowColFirst[2]; //Row and colunm of first in key
  int rowColSecond[2]; //Row and column of second in key
  int *rowCol; //Pointer to get row and column

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
  //Define variables
  /*const char key[MAXROWS][MAXCOLS] = { {'b', '4', 'd', 'n', 'F', 'z', '8', 'g'},
                                       {'B', 'u', 'f', 'm', 'v', 'X', 'w', '0'},
                                       {'I', 's', 'P', 'c', 'q', 'O', 'i', 'T'},
                                       {'7', 'a', 'r', 'l', 'M', 'Z', 'R', '3'},
                                       {'L', 'S', 'o', 'C', '.', 'V', 'x', 'Q'},
                                       {'5', 'k', 'y', 'e', 'K', 'D' ,'j', '1'},
                                       {'p', 't', 'N', 'J', ' ', '9', 'E', '6'},
                                       {'Y', 'W', 'H', 'U', '2', 'A', 'h', 'G'} };
   */
   char key[8][8], *toEncrypt, *encryptMessage;
   size_t charcount, getline_n = 0;
   
   generate_key(key);
   //Get and test input
   puts("Enter a string to be encyrpted:");
   charcount = getline(&toEncrypt, &getline_n, stdin);
   if(charcount == -1){
     perror("Error: Unalbe to get input.");
     exit(EXIT_FAILURE);
   }

   //Get lenght of input and create memory for encyrpted message
   int toEncryptLength = strlen(toEncrypt);
   toEncrypt[toEncryptLength - 1] = '\0';
   toEncryptLength = strlen(toEncrypt);
   encryptMessage = malloc(toEncryptLength + 1);
   if(encryptMessage == NULL){
     perror("Error: Memory not allocate");
     exit(EXIT_FAILURE);
   }

   //Encrypt message by pairs
   for(int i = 0; i < (toEncryptLength - 1); i += 2){
     encryptPair(toEncrypt[i], toEncrypt[i + 1], &encryptMessage[i],
                 &encryptMessage[i + 1], key);
   }

   //Set null char at end of encryptMessage string
   if(toEncryptLength % 2 == 0){
     encryptMessage[toEncryptLength] = '\0';
   } else {
     encryptMessage[toEncryptLength - 1] = toEncrypt[toEncryptLength - 1];
     encryptMessage[toEncryptLength] = '\0';
   }

   //Print original text and encyrpted text.
   printf("Original Text: %s\n", toEncrypt);
   puts("Using key:");
   for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
      printf("%c ", key[i][j]);
    }
   puts("");
   }
   printf("Cypher Text: %s\n", encryptMessage);

   //Clean heap
   free(toEncrypt);
   free(encryptMessage);
   //Close
   return EXIT_SUCCESS;
}
