#include <common.h>

// I could do this through non-void methods with returns in one copy,
// but this variant is slightly faster, and needs less typing in client code
<<<<<<< HEAD
#define M_common(SUFFIX, TYPE)                                                                           \
    void common_set_bit##SUFFIX(TYPE* variable, byte position) { *variable |= 1L << position; }          \
    void common_unset_bit##SUFFIX(TYPE* variable, byte position) { *variable &= ~(1L << position); }     \
    void common_toggle_bit##SUFFIX(TYPE* variable, byte position) { *variable ^= 1L << position; }       \
    void common_modify_bit##SUFFIX(TYPE* variable, byte position, bool set)                              \
    {                                                                                                    \
        set ? common_set_bit##SUFFIX(variable, position) : common_unset_bit##SUFFIX(variable, position); \
    }
=======
#define M_common(SUFFIX, TYPE)                                                 \
  void common_set_bit##SUFFIX(TYPE *variable, byte position) {                 \
    *variable |= 1L << position;                                               \
  }                                                                            \
  void common_unset_bit##SUFFIX(TYPE *variable, byte position) {               \
    *variable &= ~(1L << position);                                            \
  }                                                                            \
  void common_toggle_bit##SUFFIX(TYPE *variable, byte position) {              \
    *variable ^= 1L << position;                                               \
  }                                                                            \
  void common_modify_bit##SUFFIX(TYPE *variable, byte position, bool set) {    \
    set ? common_set_bit##SUFFIX(variable, position)                           \
        : common_unset_bit##SUFFIX(variable, position);                        \
  }
>>>>>>> master

M_common(b, byte) M_common(w, word) M_common(d, dword)
