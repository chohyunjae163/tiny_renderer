 //TGA spec: https://en.wikipedia.org/wiki/Truevision_TGA
//TGA importer

#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define HEADER_SIZE 18

struct TGAColorMapSpec {
  uint16_t first_entry_idx;
  uint16_t color_map_length;
  uint16_t color_map_entry_size;
};

struct TGAImageSpec {
  //TODO: implement
};

struct TGAHeader {
  uint8_t id_length;
  uint8_t color_map_type;
  uint8_t image_type;
  struct TGAColorMapSpec color_map_spec;
  struct TGAImageSpec image_spec;
};

struct TGAImage {
  uint8_t dummy;
};

/* 
 * @brief it takes a tga filename as input   
 */
struct TGAImage loadTGA(const char* filename){
  printf("loading tga image file");

  FILE* fptr = fopen(filename,"r");
  assert(fptr); // never trust input

  //TGA spec: https://en.wikipedia.org/wiki/Truevision_TGA

  /*
   * header 
   * Color map type 1 byte
   * ID length 1 byte
   * image type 1 byte
   * color map specification 5 bytes
   * image specificatino 10 bytes
   */
  //start reading header
  uint8_t header_data[HEADER_SIZE] = {0};
  fread(header_data,sizeof(uint8_t),HEADER_SIZE,fptr);
  assert( header_data[2] == 0x02 ); // image type 0x02 is uncompressed true-color image
  header_data[8];header_data[9];
  //todo: find width and height of the image.
  //      get pixels, width x height
  //end reading header  
  struct TGAImage image;
  fclose(fptr);
  return image;
}
