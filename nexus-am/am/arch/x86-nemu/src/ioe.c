#include <am.h>
#include <x86.h>

<<<<<<< HEAD
#define RTC_PORT 0x48  // Note that this is not standard
=======
#define RTC_PORT 0x48 // Note that this is not standard
>>>>>>> master
static unsigned long boot_time;

void _ioe_init() { boot_time = inl(RTC_PORT); }

<<<<<<< HEAD
unsigned long _uptime() { return 0; }

uint32_t* const fb = (uint32_t*)0x40000;
=======
unsigned long _uptime() { return inl(RTC_PORT) - boot_time; }

uint32_t *const fb = (uint32_t *)0x40000;
>>>>>>> master

_Screen _screen = {
    .width = 400, .height = 300,
};

<<<<<<< HEAD
extern void* memcpy(void*, const void*, int);

void _draw_rect(const uint32_t* pixels, int x, int y, int w, int h)
{
    int i;
    for (i = 0; i < _screen.width * _screen.height; i++) {
        fb[i] = i;
    }
=======
const size_t get_screen_width() { return _screen.width; }
const size_t get_screen_height() { return _screen.height; }

extern void *memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  int i;

  if (x >= _screen.width || y >= _screen.height)
    return;
  if (x + w > _screen.width)
    w = _screen.width - x;
  if (y + h > _screen.height)
    h = _screen.height - y;
  if (w <= 0 || h <= 0)
    return;

  for (i = 0; i < h; ++i) {
    uint32_t *dst = &fb[(y + i) * _screen.width + x];
    const uint32_t *src = &pixels[i * w];

    memcpy(dst, src, w * sizeof(uint32_t));
  }
>>>>>>> master
}

void _draw_sync() {}

<<<<<<< HEAD
int _read_key() { return _KEY_NONE; }
=======
int _read_key() {
  if (inb(0x64))
    return inl(0x60);
  return _KEY_NONE;
}
>>>>>>> master
