#include <assert.h>
#include <ndl.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap *)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  assert(bmp->pixels);
  printf("bmp->size: %d x %d\n", bmp->w, bmp->h);
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("NDL_OpenDisplay done\n");
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
