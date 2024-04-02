#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  // Check if there are any arguments passed (excluding the program name)
  if (argc <= 1) {
    printf("No arguments provided.\n");
    return 1; // Indicate error (optional)
  }

  // Loop through arguments starting from index 1 (skip program name)
  for (int i = 1; i < argc; i++) {
    printf("Argument %d: %s\n", i, argv[i]);
  }

  return 0; // Indicate successful execution
}