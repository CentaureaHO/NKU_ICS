#include "game.h"

/* a-z对应的键盘扫描码 */

static int letter_code[] = {
<<<<<<< HEAD
    _KEY_A,
    _KEY_B,
    _KEY_C,
    _KEY_D,
    _KEY_E,
    _KEY_F,
    _KEY_G,
    _KEY_H,
    _KEY_I,
    _KEY_J,
    _KEY_K,
    _KEY_L,
    _KEY_M,
    _KEY_N,
    _KEY_O,
    _KEY_P,
    _KEY_Q,
    _KEY_R,
    _KEY_S,
    _KEY_T,
    _KEY_U,
    _KEY_V,
    _KEY_W,
    _KEY_X,
    _KEY_Y,
    _KEY_Z,
=======
    _KEY_A, _KEY_B, _KEY_C, _KEY_D, _KEY_E, _KEY_F, _KEY_G, _KEY_H, _KEY_I,
    _KEY_J, _KEY_K, _KEY_L, _KEY_M, _KEY_N, _KEY_O, _KEY_P, _KEY_Q, _KEY_R,
    _KEY_S, _KEY_T, _KEY_U, _KEY_V, _KEY_W, _KEY_X, _KEY_Y, _KEY_Z,
>>>>>>> master
};

/* 对应键按下的标志位 */
static bool letter_pressed[26];

<<<<<<< HEAD
void press_key(int scan_code)
{
    int i;
    for (i = 0; i < 26; i++) {
        if (letter_code[i] == scan_code) {
            letter_pressed[i] = true;
        }
=======
void press_key(int scan_code) {
  int i;
  for (i = 0; i < 26; i++) {
    if (letter_code[i] == scan_code) {
      letter_pressed[i] = true;
>>>>>>> master
    }
}

void release_key(int index) { letter_pressed[index] = false; }

bool query_key(int index) { return letter_pressed[index]; }

static int key_code = 0;

int last_key_code(void) { return key_code; }

#define KEYDOWN_MASK 0x8000
<<<<<<< HEAD
bool keyboard_event()
{
    int keycode = _read_key();
    if (keycode == _KEY_NONE) return false;

    if ((keycode & KEYDOWN_MASK) != 0) {
        key_code = keycode & ~KEYDOWN_MASK;
        press_key(key_code);
        return true;
    }
    else
    {
        for (int i = 0; i < 26; i++) {
            if (keycode == letter_code[i]) {
                release_key(i);
            }
        }
        return true;
=======
bool keyboard_event() {
  int keycode = _read_key();
  if (keycode == _KEY_NONE)
    return false;

  if ((keycode & KEYDOWN_MASK) != 0) {
    key_code = keycode & ~KEYDOWN_MASK;
    press_key(key_code);
    return true;
  } else {
    for (int i = 0; i < 26; i++) {
      if (keycode == letter_code[i]) {
        release_key(i);
      }
>>>>>>> master
    }
}
