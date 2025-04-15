#include <am.h>
#include <klib.h>

#define NAME(key) [_KEY_##key] = #key,

<<<<<<< HEAD
const char* names[256] = {[_KEY_NONE] = "NONE", _KEYS(NAME)};
int         main()
{
    _ioe_init();
=======
const char *names[256] = {[_KEY_NONE] = "NONE", _KEYS(NAME)};
int main() {
  _ioe_init();
>>>>>>> master

    while (1) {
        int  key  = _read_key();
        bool down = false;
        if (key & 0x8000) {
            key ^= 0x8000;
            down = true;
        }
        if (key != _KEY_NONE) {
            printf("Get key: %d %s %s\n", key, names[key], down ? "down" : "up");
        }
    }
    return 0;
}
