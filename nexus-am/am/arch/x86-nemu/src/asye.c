#include <am.h>
#include <x86.h>

static _RegSet *(*H)(_Event, _RegSet *) = NULL;

void vecsys();
void vecnull();

_RegSet *irq_handle(_RegSet *tf) {
  _RegSet *next = tf;
  if (H) {
    _Event ev;
    switch (tf->irq) {
    case 0x80:
      ev.event = _EVENT_SYSCALL;
      break;
    default:
      ev.event = _EVENT_ERROR;
      break;
    }

    next = H(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }

  return next;
}

static GateDesc idt[NR_IRQ];

#include <am.h>

void print(const char *s) {
  for (; *s; s++) {
    _putc(*s);
  }
}

void print_hex(uint32_t val) {
  char hex_chars[] = "0123456789ABCDEF";
  _putc('0'); _putc('x');
  for (int i = 7; i >= 0; i--) {
    _putc(hex_chars[(val >> (i * 4)) & 0xF]);
  }
}

void print_dec(int val) {
  if (val < 0) {
    _putc('-');
    val = -val;
  }
  
  if (val >= 10) print_dec(val / 10);
  _putc('0' + (val % 10));
}

void _asye_init(_RegSet *(*h)(_Event, _RegSet *)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x80] = GATE(STS_TG32, KSEL(SEG_KCODE), vecsys, DPL_USER);

  print("IDT address: "); print_hex((uint32_t)idt); print("\n");
  print("IDT size: "); print_dec(sizeof(idt)); print("\n");
  

  print("IDT[0x80] before set_idt:\n");
  print("  off_low: "); print_hex(idt[0x80].off_15_0); print("\n");
  print("  cs: "); print_hex(idt[0x80].cs); print("\n");
  print("  off_high: "); print_hex(idt[0x80].off_31_16); print("\n");


  set_idt(idt, sizeof(idt));

  // register event handler
  H = h;
}

_RegSet *_make(_Area stack, void *entry, void *arg) { return NULL; }

void _trap() {}

int _istatus(int enable) { return 0; }
