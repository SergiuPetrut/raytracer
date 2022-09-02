//Outputs data as a PPM
#include "ppmoutput.h"


void saveppm(char * filename, unsigned char * img, int width, int height) {
  // FILE pointer
  FILE * f;

  //using fopen in order to write to the file
  f = fopen(filename, "wb");

  // PPM header info + img size
  fprintf(f, "P6 %d %d %d\n", width, height, 255);

  // Writes the image data to the file, 3 pixels at a time
  fwrite(img, 3, width * height, f);

  //Closes the file after done with writing data to it
  fclose(f);
}
