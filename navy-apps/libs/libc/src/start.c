#include <assert.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]);

<<<<<<< HEAD
__attribute__((section(".text.unlikely"))) void _start(int argc, char* argv[], char* envp[])
{
    int ret = main(argc, argv, envp);
    exit(ret);
    assert(0);
=======
__attribute__((section(".text.unlikely"))) void _start(int argc, char *argv[],
                                                       char *envp[]) {
  int ret = main(argc, argv, envp);
  exit(ret);
  assert(0);
>>>>>>> master
}
