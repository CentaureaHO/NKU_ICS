#include <assert.h>
#include <stdio.h>

int main()
{
    FILE* fp = fopen("/share/texts/num", "r+");
    assert(fp);

<<<<<<< HEAD
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    assert(size == 5000);

    fseek(fp, 500 * 5, SEEK_SET);
    int i, n;
    for (i = 500; i < 1000; i++) {
        fscanf(fp, "%d", &n);
        assert(n == i + 1);
    }

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < 500; i++) {
        fprintf(fp, "%4d\n", i + 1 + 1000);
    }

    for (i = 500; i < 1000; i++) {
        fscanf(fp, "%d", &n);
        assert(n == i + 1);
    }

    fseek(fp, 0, SEEK_SET);
    for (i = 0; i < 500; i++) {
        fscanf(fp, "%d", &n);
        assert(n == i + 1 + 1000);
    }
=======
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  printf("size = %ld\n", size);
  assert(size == 5000);

  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  for (i = 500; i < 1000; i++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }

  for (i = 500; i < 1000; i++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }
>>>>>>> master

    fclose(fp);

    printf("PASS!!!\n");

    return 0;
}
