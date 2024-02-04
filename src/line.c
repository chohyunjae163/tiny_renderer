/***
* line drawing function
*/

#include <stdint.h>

uint32_t color = UINT32_MAX;

void horizontal_line(void* buffer, int x0, int x1, int y) { 
  for ( uint32_t x = x0; x <= x1; ++x ) {
    pixel(buffer,x,y,color);
  }
}

void vertical_line(void* buffer, int y0, int y1, int x) {
  for ( uint32_t y = y0 ; y <= y1 ; ++y ){
    pixel(buffer,x,y,color);
  }
}


void bresenham(void* buffer, int x0, int x1, int y0, int y1){
  uint16_t dy = y1 - y0;
  uint16_t dx = x1 - x0;
  int16_t p = 2 * dy - dx;
  uint16_t y = y0;
  uint16_t x = x0;
  for ( ; x <= x1; ++x ) {
    pixel(buffer,x,y,color);
    if ( p > 0 ) {
      p = p + 2 * dy - 2 * dx;
      ++y;
    }
    else {
      p = p + 2 * dy;
    }
  }
}
