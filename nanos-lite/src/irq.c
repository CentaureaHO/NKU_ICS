#include "common.h"

static _RegSet *do_event(_Event e, _RegSet *r) {
    switch (e.event) 
    {
      case _EVENT_SYSCALL:
        Log("System call: %d", r->eax);
        TODO();
        break;
      default:
        panic("Unhandled event ID = %d", e.event);
    }

  return NULL;
}

void init_irq(void) 
{ 
    Log("Initializing interrupt/exception handler...");
    _asye_init(do_event); 
}
