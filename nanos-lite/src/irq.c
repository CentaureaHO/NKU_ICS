#include "common.h"

<<<<<<< HEAD
static _RegSet* do_event(_Event e, _RegSet* r)
{
    switch (e.event)
    {
        default: panic("Unhandled event ID = %d", e.event);
    }
=======
_RegSet *do_syscall(_RegSet *r);

static _RegSet *do_event(_Event e, _RegSet *r) {
  switch (e.event) {
  case _EVENT_SYSCALL:
    do_syscall(r);
    break;
  default:
    panic("Unhandled event ID = %d", e.event);
  }
>>>>>>> master

    return NULL;
}

<<<<<<< HEAD
void init_irq(void) { _asye_init(do_event); }
=======
void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _asye_init(do_event);
}
>>>>>>> master
