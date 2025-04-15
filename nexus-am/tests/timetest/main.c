#include <am.h>
#include <klib.h>

<<<<<<< HEAD
int main()
{
    _ioe_init();
    int sec = 1;
    while (1) {
        while (_uptime() < 1000 * sec)
            ;
        if (sec == 1) {
            printf("%d second.\n", sec);
        }
        else
        {
            printf("%d seconds.\n", sec);
        }
        sec++;
    }
    return 0;
=======
int main() {
  _ioe_init();
  int sec = 1;
  while (1) {
    while (_uptime() < 1000 * sec)
      ;
    if (sec == 1) {
      printf("%d second.\n", sec);
    } else {
      printf("%d seconds.\n", sec);
    }
    sec++;
  }
  return 0;
>>>>>>> master
}
