#include "common.h"

#define NAME(key) [_KEY_##key] = #key,

extern const size_t get_screen_width();
extern const size_t get_screen_height();

static const char* keyname[256] __attribute__((used)) = {[_KEY_NONE] = "NONE", _KEYS(NAME)};

#if SCHEDULE_POLICY == 3
extern int curgame_idx;  // 0 for pal, 1 for videotest
#endif

static char buffer[64] __attribute__((used));
size_t events_read(void* buf, size_t len)
{
    int  key  = _read_key();
    bool down = false;
    if (key & 0x8000) {
        key ^= 0x8000;
        down = true;
    }

#if SCHEDULE_POLICY == 3
    if (down && key == _KEY_F12) curgame_idx = 1 - curgame_idx;
#endif

    if (key != _KEY_NONE)
        sprintf(buffer, "%s %s\n", down ? "kd" : "ku", keyname[key]);
    else
        sprintf(buffer, "t %d\n", _uptime());

    size_t ret = strlen(buffer) > len ? len : strlen(buffer);
    memcpy(buf, buffer, ret);
    return ret;
}

static char dispinfo[128] __attribute__((used));

ssize_t dispinfo_read(void* buf, off_t offset, size_t len)
{
    if (offset >= strlen(dispinfo)) return 0;

    if (offset + len > strlen(dispinfo)) len = strlen(dispinfo) - offset;

    memcpy(buf, dispinfo + offset, len);
    return len;
}

void fb_write(const void* buf, off_t offset, size_t len)
{
    const size_t width = get_screen_width();
    int          x     = (offset / sizeof(uint32_t)) % width;
    int          y     = (offset / sizeof(uint32_t)) / width;

    _draw_rect((uint32_t*)buf, x, y, len / sizeof(uint32_t), 1);
}

void init_device()
{
    _ioe_init();

    // TODO: print the string to array `dispinfo` with the format
    // described in the Navy-apps convention

    const size_t width  = get_screen_width();
    const size_t height = get_screen_height();
    sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d", width, height);
    printf("%s", dispinfo);
}
