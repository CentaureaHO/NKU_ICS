#include <stdio.h>
#include <unistd.h>

<<<<<<< HEAD
int main()
{
    write(1, "Hello World!\n", 13);
    int          i = 2;
    volatile int j = 0;
    while (1) {
        j++;
        if (j == 10000) {
            printf("Hello World for the %dth time\n", i++);
            j = 0;
        }
=======
int main() {
  write(1, "Hello World!\n", 13);
  int i = 2;
  volatile int j = 0;
  while (1) {
    j++;
    if (j == 10000) {
      printf("Hello World for the %dth time\n", i++);
      j = 0;
>>>>>>> master
    }
    return 0;
}
