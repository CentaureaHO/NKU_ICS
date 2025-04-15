#include <assert.h>
#include <ndl.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap *)malloc(sizeof(NDL_Bitmap));
  printf("bmp: %p\n", bmp);
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  printf("bmp->pixels: %p\n", bmp->pixels);
  assert(bmp->pixels);
  printf("bmp->w: %d\n", bmp->w);
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("bmp->h: %d\n", bmp->h);
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  printf("NDL_DrawRect done\n");
  NDL_Render();
  printf("NDL_Render done\n");
  NDL_CloseDisplay();
  printf("NDL_CloseDisplay done\n");
  while (1)
    ;
  return 0;
}
