#include <assert.h>
#include <ndl.h>
<<<<<<< HEAD
#include <stdlib.h>

int main()
{
    NDL_Bitmap* bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
    NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
    assert(bmp->pixels);
    NDL_OpenDisplay(bmp->w, bmp->h);
    NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
    NDL_Render();
    NDL_CloseDisplay();
    while (1)
        ;
    return 0;
=======
#include <stdio.h>
#include <stdlib.h>

int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap *)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  NDL_Render();
  NDL_CloseDisplay();
  while (1)
    ;
  return 0;
>>>>>>> master
}
