#include "fs.h"

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum {
  FD_STDIN,
  FD_STDOUT,
  FD_STDERR,
  FD_FB,
  FD_EVENTS,
  FD_DISPINFO,
  FD_NORMAL
};

enum { SEEK_SET, SEEK_CUR, SEEK_END };

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    {"stdin (note that this is not the actual stdin)", 0, 0},
    {"stdout (note that this is not the actual stdout)", 0, 0},
    {"stderr (note that this is not the actual stderr)", 0, 0},
    [FD_FB] = {"/dev/fb", 0, 0},
    [FD_EVENTS] = {"/dev/events", 0, 0},
    [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern void ramdisk_write(const void *buf, off_t offset, size_t len);
extern ssize_t dispinfo_read(void *buf, off_t offset, size_t len);
extern void fb_write(const void *buf, off_t offset, size_t len);
extern size_t events_read(void *buf, size_t len);

extern const size_t get_screen_width();
extern const size_t get_screen_height();

void init_fs() {
  // TODO: initialize the size of /dev/fb
  const size_t screen_width = get_screen_width();
  const size_t screen_height = get_screen_height();
  Log("screen size: %d x %d", screen_width, screen_height);

  file_table[FD_FB].size = screen_width * screen_height * sizeof(uint32_t);
}

int fs_open(const char *pathname, int flags, int mode) {
  for (int i = 0; i < NR_FILES; i++)
    if (strcmp(pathname, file_table[i].name) == 0) {
      file_table[i].open_offset = 0;
      return i;
    }

  panic("File %s not found", pathname);
  return -1;
}

ssize_t fs_read(int fd, void *buf, size_t len) {
  ssize_t read_len = 0;

  if (fd < 0 || fd >= NR_FILES)
    panic("Invalid file descriptor: %d", fd);

  switch (fd) {
  case FD_STDIN:
  case FD_STDOUT:
  case FD_STDERR:
    return 0;
  case FD_FB:
    panic("Not implemented for read FD_FB");
  case FD_EVENTS:
    read_len = events_read(buf, len);
    return read_len;
  case FD_DISPINFO:
    read_len = dispinfo_read(buf, file_table[fd].open_offset, len);
    file_table[fd].open_offset += read_len;
    return read_len;
  case FD_NORMAL:
    panic("Not implemented for read FD_NORMAL");
  default:
    break;
  }

  size_t remain_bytes = file_table[fd].size - file_table[fd].open_offset;
  if (remain_bytes < len)
    len = remain_bytes;

  if (len > 0) {
    ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset,
                 len);
    file_table[fd].open_offset += len;
  }

  return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len) {
  if (fd < 0 || fd >= NR_FILES)
    panic("Invalid file descriptor: %d", fd);

  switch (fd) {
  case FD_STDIN:
    return 0;
  case FD_STDOUT:
  case FD_STDERR:
    for (size_t i = 0; i < len; i++)
      _putc(((char *)buf)[i]);
    return len;
  case FD_FB:
    fb_write(buf, file_table[fd].open_offset, len);
    file_table[fd].open_offset += len;
    return len;
  case FD_EVENTS:
    panic("Not implemented for read FD_EVENTS");
  case FD_DISPINFO:
    panic("Not implemented for read FD_DISPINFO");
  case FD_NORMAL:
    panic("Not implemented for read FD_NORMAL");
  default:
    break;
  }

  size_t remain_bytes = file_table[fd].size - file_table[fd].open_offset;
  if (remain_bytes < len)
    len = remain_bytes;

  if (len > 0) {
    ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset,
                  len);
    file_table[fd].open_offset += len;
  }

  return len;
}

off_t fs_lseek(int fd, off_t offset, int whence) {
  if (fd < 0 || fd >= NR_FILES)
    panic("Invalid file descriptor: %d", fd);

  switch (whence) {
  case SEEK_SET:
    file_table[fd].open_offset = offset;
    break;
  case SEEK_CUR:
    file_table[fd].open_offset += offset;
    break;
  case SEEK_END:
    file_table[fd].open_offset = file_table[fd].size + offset;
    break;
  default:
    panic("Unsupported whence: %d", whence);
  }

  if (file_table[fd].open_offset < 0)
    file_table[fd].open_offset = 0;
  if (file_table[fd].open_offset > file_table[fd].size)
    file_table[fd].open_offset = file_table[fd].size;

  return file_table[fd].open_offset;
}

int fs_close(int fd) {
  if (fd < 0 || fd >= NR_FILES)
    panic("Invalid file descriptor: %d", fd);

  return 0;
}

size_t fs_filesz(int fd) {
  if (fd < 0 || fd >= NR_FILES)
    panic("Invalid file descriptor: %d", fd);

  return file_table[fd].size;
}
