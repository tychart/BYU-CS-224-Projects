#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0

#define BAD_NUMBER_ARGS 1
#define BAD_OPTION 2
#define FSEEK_ERROR 3
#define FREAD_ERROR 4
#define MALLOC_ERROR 5
#define FWRITE_ERROR 6

/**
 * Parses the command line.
 *
 * argc:      the number of items on the command line (and length of the
 *            argv array) including the executable
 * argv:      the array of arguments as strings (char* array)
 * grayscale: the integer value is set to TRUE if grayscale output indicated
 *            outherwise FALSE for threshold output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *isGrayscale) {
  if (argc > 2) {
    fprintf(stderr, "Usage: %s [-g]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2) {
    if (strcmp(argv[1], "-g") == 0) {
      *isGrayscale = TRUE;

    } else if (strcmp(argv[1], "-s") == 0) {
      // set isscale here

    } else {
      fprintf(stderr, "Unknown option: '%s'\n", argv[1]);
      fprintf(stderr, "Usage: %s [-g]\n", argv[0]);
      exit(BAD_OPTION);  
    }
  }

  return stdin;
}

unsigned getFileSizeInBytes(FILE* stream) {
  unsigned fileSizeInBytes = 0;
  
  rewind(stream);
  if (fseek(stream, 0L, SEEK_END) != 0) {
    exit(FSEEK_ERROR);
  }
  fileSizeInBytes = ftell(stream);

  return fileSizeInBytes;
}

void getBmpFileAsBytes(unsigned char* ptr, unsigned fileSizeInBytes, FILE* stream) {
  rewind(stream);
  if (fread(ptr, fileSizeInBytes, 1, stream) != 1) {
#ifdef DEBUG
    printf("feof() = %x\n", feof(stream));
    printf("ferror() = %x\n", ferror(stream));
#endif
    exit(FREAD_ERROR);
  }
}

unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red) {
  printf("TODO: unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red)\n");
  return 0;
}

void applyGrayscaleToPixel(unsigned char* pixel) {
  printf("TODO: void applyGrayscaleToPixel(unsigned char* pixel)\n");
}

void applyThresholdToPixel(unsigned char* pixel) {
  // printf("TODO: void applyThresholdToPixel(unsigned char* pixel) Pixel=%x\n", pixel);

  
  

  #ifdef DEBUG
    unsigned char red = pixel[0];
    unsigned char green = pixel[1];
    unsigned char blue = pixel[2];

    printf("Red=%d, Green=%d, Blue=%d\n", red, green, blue);
  #endif

  
  #ifdef DEBUG
    printf("currPixel = %p\n", pixel);
  #endif  

  if(((unsigned char) pixel[0] + (unsigned char) pixel[1] + (unsigned char) pixel[2]) / 3 > 128) {
    pixel[0] = 0xff;
    pixel[1] = 0xff;
    pixel[2] = 0xff;
  }else{
    pixel[0] = 0x00;
    pixel[1] = 0x00;
    pixel[2] = 0x00;
  }

  

  
  #ifdef DEBUG
    red = pixel[0];
    green = pixel[1];
    blue = pixel[2];
    
    printf("Adjusted: Red=%d, Green=%d, Blue=%d\n", red, green, blue);
  #endif  

}

void applyFilterToPixel(unsigned char* pixel, int isGrayscale) {
  // printf("TODO: void applyFilterToPixel(unsigned char* pixel, int isGrayscale)\n");

  if(isGrayscale) {
    applyGrayscaleToPixel(pixel);
    return;
  }

  applyThresholdToPixel(pixel);

}

void applyFilterToRow(unsigned char* row, int width, int isGrayscale) {
  // printf("TODO: void applyFilterToRow(unsigned char* row, int width, int isGrayscale)\n");

  #ifdef DEBUG
    printf("currRow = %p\n", row);
  #endif  

  for(int currCol = 0; currCol < width; currCol += 3) {
    applyFilterToPixel(row + currCol, isGrayscale);
  }
}

void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale) {
  int padding = 0;
  // printf("TODO: compute the required amount of padding from the image width/n");
  padding = width % 4;

#ifdef DEBUG
  printf("padding = %d\n", padding);
  printf("starting = %p\n", pixelArray);
#endif  
  
  // printf("TODO: void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale)\n");

  unsigned char* paddedPixelArray = pixelArray;

  for(int currHeight = 0; currHeight < height; currHeight++) {
    applyFilterToRow(paddedPixelArray + (width * currHeight), width, isGrayscale);
    paddedPixelArray += padding;
  }

}

void parseHeaderAndApplyFilter(unsigned char* bmpFileAsBytes, int isGrayscale) {
  int offsetFirstBytePixelArray = 0;
  int width = 0;
  int height = 0;
  unsigned char* pixelArray = NULL;

  
  // Set offset for first pixle in array
  offsetFirstBytePixelArray = bmpFileAsBytes[10] | bmpFileAsBytes[11] << 8 | bmpFileAsBytes[12] << 16 | bmpFileAsBytes[13] << 24;
  // printf("charsasHex: %x %x %x %x\n", bmpFileAsBytes[3], bmpFileAsBytes[4], bmpFileAsBytes[5], bmpFileAsBytes[6]);
  // printf("TODO: set offsetFirstBytePixelArray=%d\n", offsetFirstBytePixelArray);

  // Set Width
  width = bmpFileAsBytes[18] | bmpFileAsBytes[19] << 8 | bmpFileAsBytes[20] << 16 | bmpFileAsBytes[21] << 24;

  // printf("TODO: se/t width=%d\n", width);

  // Set Height
  height = bmpFileAsBytes[22] | bmpFileAsBytes[23] << 8 | bmpFileAsBytes[24] << 16 | bmpFileAsBytes[25] << 24;

  // printf("TODO: set height=%d\n", height);


  // set the pixelArray to the start of the pixel array

  pixelArray = bmpFileAsBytes + offsetFirstBytePixelArray;

#ifdef DEBUG
  printf("offsetFirstBytePixelArray = %u\n", offsetFirstBytePixelArray);
  printf("width = %u\n", width);
  printf("height = %u\n", height);
  printf("bmpArray = %p\n", bmpFileAsBytes);
  printf("pixelArray = %p\n", pixelArray);
#endif

  applyFilterToPixelArray(pixelArray, width, height, isGrayscale);
}

int main(int argc, char **argv) {
  int grayscale = FALSE;
  unsigned fileSizeInBytes = 0;
  unsigned char* bmpFileAsBytes = NULL;
  FILE *stream = NULL;
  
  stream = parseCommandLine(argc, argv, &grayscale);
  fileSizeInBytes = getFileSizeInBytes(stream);

#ifdef DEBUG
  printf("fileSizeInBytes = %u\n", fileSizeInBytes);
#endif

  bmpFileAsBytes = (unsigned char *)malloc(fileSizeInBytes);
  if (bmpFileAsBytes == NULL) {
    exit(MALLOC_ERROR);
  }
  getBmpFileAsBytes(bmpFileAsBytes, fileSizeInBytes, stream);

  parseHeaderAndApplyFilter(bmpFileAsBytes, grayscale);

#ifndef DEBUG
  if (fwrite(bmpFileAsBytes, fileSizeInBytes, 1, stdout) != 1) {
    exit(FWRITE_ERROR);
  }
#endif

  free(bmpFileAsBytes);
  return 0;
}
