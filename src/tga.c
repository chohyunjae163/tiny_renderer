#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#define HEADER_SIZE 18

#pragma pack(1)
struct ColorMapSpec {
  uint16_t first_entry_idx;
  uint16_t color_map_length;
  uint8_t color_map_entry_size;
};



struct ImageSpec {
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint8_t pixel_depth; // bits per pixel
  uint8_t descriptor; // bits 3-0 give the alpha channel depth, bits 5-4 give pixel ordering
};

struct Header {
  uint8_t id_length;
  uint8_t color_map_type;
  uint8_t image_type;
  struct ColorMapSpec color_map_spec;
  struct ImageSpec image_spec;
};
#pragma pack()
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
  struct Header header = { 0 };
  const uint8_t header_size = sizeof(struct Header);
  assert(header_size == HEADER_SIZE);
  fread(&header,header_size,1,fptr);
  assert( header.image_type == 0x02 ); // image type 0x02 is uncompressed true-color image

  //todo: find width and height of the image.
  //      get pixels, width x height
  //end reading header  
  struct TGAImage image;
  fclose(fptr);
  return image;
}
