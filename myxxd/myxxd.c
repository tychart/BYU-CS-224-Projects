#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
  // printf("TODO 1: printDataAsHex (2)");
  char hexCharArr[2] = {'\x00', '\x00'};

  printf(" ");

  for(int i = 0; i < size; i++) {
    sprintf(hexCharArr, "%x", data[i]);
    
    // if(atoi(hexCharArr) < 8 && i != size - 1) {
    if(hexCharArr[1] == '\x00') {
      
      printf("0");
    }
    // int result = (unsigned char)hexCharArr[0] | ((unsigned char)hexCharArr[1] << 8);
    // int result2 = ((unsigned char)hexCharArr[1] << 8) | (unsigned char)hexCharArr[0];
    printf("%s", hexCharArr);
    // printf("(int=%d)", result2);
    
    if(i % 2 == 1 && i != size - 1) {
      printf(" ");
    }
  }
  
  // printf("data:%x", output);
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
  // printf("TODO 2: printDataAsChars (3)");

  for(int i = 0; i < size; i++) {
    char outCharArr[2];
    sprintf(outCharArr, "%c", data[i]);
    if(isprint(outCharArr[0])) {
      printf("%c", data[i]);
    } else {
      printf(".");
    }

    

  }

}

void readAndPrintInputAsHex(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    if(numBytesRead != 16) {
      // for(int i = 0; i < 6-lineLen; i++) {
      for(int i = 0; i < 16-numBytesRead; i++) {
        printf("  ");
        if(i % 2 == 1) {
          printf(" ");
        }
      }
    }
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  // printf("TODO 3: readAndPrintInputAsBits\n");
  
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;
  int lineLen = 0;

  while (numBytesRead != 0) {
    lineLen = 0;
    printf("%08x:", offset);
    printf(" ");

    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf("  ");
    if(numBytesRead != 6) {
      // for(int i = 0; i < 6-lineLen; i++) {
      for(int i = 0; i < 6-numBytesRead; i++) {
        printf("         ");
      }
    }
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }

}

void printDataAsBits(unsigned char *data, size_t size) {

  int counter = 0;

  for(int i = 0; i < size / 2; i++) {

    for(int j = 0; j < 2; j++) {
      
      printCharAsBits(data[counter]);
      if(counter != size - 1) {
        printf(" ");
      }
      counter++;
    }
    

  }

}

void printCharAsBits(unsigned char inChar) {
    // This is our char being converted
    unsigned char c = inChar;
    
    // You'll also need an array to store the bit string,
    // either chars or ints will work

    int intArr[8];

    for(int i = 0; i < 8; i++) {
      
      if(c % 2 == 1) {
        intArr[i] = 1;
      } else {
        intArr[i] = 0;
      }

      c = c / 2;

    }
    
    // Print out the resulting binary number
    // printf("Character In Binary\n");

    for(int i = 7; i >= 0; i--) {
    
      printf("%d", intArr[i]);

    }

    // printf("\n");
    
    return;
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
