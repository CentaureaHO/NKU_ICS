#include "common.h"

#define NAME(key) [_KEY_##key] = #key,

extern const size_t get_screen_width();
extern const size_t get_screen_height();

static const char *keyname[256]
    __attribute__((used)) = {[_KEY_NONE] = "NONE", _KEYS(NAME)};

size_t events_read(void *buf, size_t len) { return 0; }

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) 
{
  ssize_t it = 0;
  
  char* buffer = (char*)buf;
  for (; it < len && offset + it < 128; ++it)
    buffer[it] = dispinfo[offset + it];
}

void fb_write(const void *buf, off_t offset, size_t len) 
{
  const size_t width = get_screen_width();
  int x = (offset / sizeof(uint32_t)) % width;
  int y = (offset / sizeof(uint32_t)) / width;

  _draw_rect((uint32_t *)buf, x, y, len / sizeof(uint32_t), 1);
}

void init_device() {
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention

  const size_t width = get_screen_width();
  const size_t height = get_screen_height();
  Log("width = %d, height = %d", width, height);
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", width, height);
}
