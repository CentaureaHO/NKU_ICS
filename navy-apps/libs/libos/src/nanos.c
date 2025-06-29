#include "syscall.h"
#include <assert.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

// TODO: discuss with syscall interface
#ifndef __ISA_NATIVE__

// FIXME: this is temporary

<<<<<<< HEAD
int _syscall_(int type, uintptr_t a0, uintptr_t a1, uintptr_t a2)
{
    int ret = -1;
    asm volatile("int $0x80" : "=a"(ret) : "a"(type), "b"(a0), "c"(a1), "d"(a2));
    return ret;
}

void _exit(int status) { _syscall_(SYS_exit, status, 0, 0); }

int _open(const char* path, int flags, mode_t mode) { _exit(SYS_open); }

int _write(int fd, void* buf, size_t count) { _exit(SYS_write); }

void* _sbrk(intptr_t increment) { return (void*)-1; }

int _read(int fd, void* buf, size_t count) { _exit(SYS_read); }

int _close(int fd) { _exit(SYS_close); }

off_t _lseek(int fd, off_t offset, int whence) { _exit(SYS_lseek); }
=======
int _syscall_(int type, uintptr_t a0, uintptr_t a1, uintptr_t a2) {
  int ret = -1;
  asm volatile("int $0x80" : "=a"(ret) : "a"(type), "b"(a0), "c"(a1), "d"(a2));
  return ret;
}

static int _unimplemented(int syscall_type) {
  return _syscall_(SYS_undone, syscall_type, 0, 0);
}

void _exit(int status) { _syscall_(SYS_exit, status, 0, 0); }

int _open(const char *path, int flags, mode_t mode) {
  return _syscall_(SYS_open, (uintptr_t)path, flags, mode);
}

int _write(int fd, void *buf, size_t count) {
  // 不是哥们，指导书就一句话
  // 扫一眼就过去了
  // 牛魔的我找了半天
  // 就不能直接换个专门提示未实现的调用？
  return _syscall_(SYS_write, fd, (uintptr_t)buf, count);
}

extern char end;

void *_sbrk(intptr_t increment) {
  static intptr_t program_break = 0;
  intptr_t old_break;

  if (program_break == 0)
    program_break = (intptr_t)&end;

  old_break = program_break;
  intptr_t new_break = old_break + increment;

  if (_syscall_(SYS_brk, new_break, 0, 0) == 0) {
    program_break = new_break;
    return (void *)old_break;
  }

  return (void *)-1;
}

int _read(int fd, void *buf, size_t count) {
  return _syscall_(SYS_read, fd, (uintptr_t)buf, count);
}

int _close(int fd) { return _syscall_(SYS_close, fd, 0, 0); }

off_t _lseek(int fd, off_t offset, int whence) {
  return _syscall_(SYS_lseek, fd, offset, whence);
}
>>>>>>> master

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

// not implement but used
<<<<<<< HEAD
int _fstat(int fd, struct stat* buf) { return 0; }

int execve(const char* fname, char* const argv[], char* const envp[])
{
    assert(0);
    return -1;
}

int _execve(const char* fname, char* const argv[], char* const envp[]) { return execve(fname, argv, envp); }

int _kill(int pid, int sig)
{
    _exit(-SYS_kill);
    return -1;
}

pid_t _getpid()
{
    _exit(-SYS_getpid);
    return 1;
}

char** environ;

time_t time(time_t* tloc)
{
    assert(0);
    return 0;
}

int signal(int num, void* handler)
{
    assert(0);
    return -1;
=======
int _fstat(int fd, struct stat *buf) { return 0; }

int execve(const char *fname, char *const argv[], char *const envp[]) {
  assert(0);
  return -1;
}

int _execve(const char *fname, char *const argv[], char *const envp[]) {
  return execve(fname, argv, envp);
}

int _kill(int pid, int sig) {
  return _unimplemented(SYS_kill);
  _exit(-SYS_kill);
  return -1;
}

pid_t _getpid() {
  return _unimplemented(SYS_getpid);
  _exit(-SYS_getpid);
  return 1;
>>>>>>> master
}

pid_t _fork()
{
    assert(0);
    return -1;
}

int _link(const char* d, const char* n)
{
    assert(0);
    return -1;
}

int _unlink(const char* n)
{
    assert(0);
    return -1;
}

pid_t _wait(int* status)
{
    assert(0);
    return -1;
}

clock_t _times(void* buf)
{
    assert(0);
    return 0;
}

int _gettimeofday(struct timeval* tv)
{
    assert(0);
    tv->tv_sec  = 0;
    tv->tv_usec = 0;
    return 0;
}

int _fcntl(int fd, int cmd, ...)
{
    assert(0);
    return 0;
}

int pipe(int pipefd[2])
{
    assert(0);
    return 0;
}

<<<<<<< HEAD
int dup(int oldfd)
{
    assert(0);
    return 0;
=======
int _fcntl(int fd, int cmd, ...) {
  assert(0);
  return 0;
>>>>>>> master
}

int dup2(int oldfd, int newfd)
{
    assert(0);
    return 0;
}

pid_t vfork(void)
{
    assert(0);
    return 0;
}

#endif
