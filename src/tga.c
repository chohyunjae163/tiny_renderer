//tga import and export
//no rle compression support. just uncompressed one.
//TGA spec: https://en.wikipedia.org/wiki/Truevision_TGA
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "memory_pool.c"

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

enum EImageSizeType {
  size_32x32 = 0,
  size_64x64,
  size_128x128,
  size_256x256,
  size_512x512,
  size_1024x1024,
  size_2048x2048,
  size_4096x4096
};

static enum EImageSizeType get_image_size(uint16_t size){
  uint8_t num_shift = 1;
  while(size > 32) {
    size = size >> 1;
    num_shift += 1;
  }

  return (enum EImageSizeType) num_shift;
}

#define NUM_IMAGE 16
static uint32_t* raw_images[NUM_IMAGE];
static uint8_t current_num_image = 0;
//0000 0000
//4 bits for image IDs
//3 bits for size
//32x32,64x64,128x128,256x256,512x512,1024x1024,2048x2048,4096x4096
typedef uint8_t ImageHandle;

/* 
 * @brief it takes a tga filename as input   
 */
ImageHandle read_tga(const char* filename){
  if( current_num_image >= NUM_IMAGE ) {
    return 0;
  }
  printf("loading tga image file\n");

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

  uint16_t width = header.image_spec.width;
  uint16_t height = header.image_spec.height;
  printf("image width : %d\n",width);
  printf("image height :%d\n",height);
  assert(width == height);

  uint8_t pixel_depth = header.image_spec.pixel_depth;
  printf("pixel depth (bits per pixel): %d\n",pixel_depth);
  assert(pixel_depth == 32);

  const uint32_t memory_required = width * height * pixel_depth / 4;
  uint32_t* image_ptr = (uint32_t*)request_memory(memory_required);
  raw_images[current_num_image++] = image_ptr;
  fread(image_ptr, memory_required, 1 , fptr);
  //todo: find width and height of the image.
  //      get pixels, width x height
  //end reading header  

  fclose(fptr);
  ImageHandle handle;
  handle |= (pixel_depth == 32) ? 1 << 8 : 0; 
  handle |= get_image_size(width) << 4;
  handle |= current_num_image;
  return handle; 
}  

uint32_t* get_raw_image(ImageHandle handle) {
  //TODO: read image handle to get raw image ptr
  //and return it
  uint8_t index = handle & 0xF; //"0x" tells the compiler that what follows is a hexadecimal number (base 16). 
  return raw_images[index];
}


// need pixel data,size,
int8_t write_tga(const char* filepath, uint16_t size,uint8_t* colours_ptr) {
  FILE* fptr = fopen(filepath,"w");
  if(fptr == NULL) {
    printf("failed opening a filepath");
    return -1;
  }

  struct Header header = {0};
  header.image_type = 0x02;
  header.image_spec.width = size;
  header.image_spec.height = size;
  header.image_spec.pixel_depth = 32;

  //write header
  size_t header_ret = fwrite(&header,sizeof(struct Header),1,fptr);
  assert(header_ret == 1);

  //write color data
  size_t color_data_size = size * size * 4;
  size_t color_ret = fwrite(&colours_ptr,color_data_size,1,fptr);
  assert(color_ret == 1);
  fclose(fptr);

  return 0;
