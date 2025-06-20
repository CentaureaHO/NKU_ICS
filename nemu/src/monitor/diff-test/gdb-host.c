#include "common.h"
#include "protocol.h"
#include <stdlib.h>
#include <unistd.h>

static struct gdb_conn* conn;

bool gdb_connect_qemu(void)
{
    // connect to gdbserver on localhost port 1234
    while ((conn = gdb_begin_inet("127.0.0.1", 1234)) == NULL) {
        usleep(1);
    }

    return true;
}

static bool gdb_memcpy_to_qemu_small(uint32_t dest, void* src, int len)
{
    char* buf = malloc(len * 2 + 128);
    assert(buf != NULL);
    int p = sprintf(buf, "M0x%x,%x:", dest, len);
    int i;
    for (i = 0; i < len; i++) {
        p += sprintf(buf + p, "%c%c", hex_encode(((uint8_t*)src)[i] >> 4), hex_encode(((uint8_t*)src)[i] & 0xf));
    }

    gdb_send(conn, (const uint8_t*)buf, strlen(buf));
    free(buf);

    size_t   size;
    uint8_t* reply = gdb_recv(conn, &size);
    bool     ok    = !strcmp((const char*)reply, "OK");
    free(reply);

    return ok;
}

bool gdb_memcpy_to_qemu(uint32_t dest, void* src, int len)
{
    const int mtu = 1500;
    bool      ok  = true;
    while (len > mtu) {
        ok &= gdb_memcpy_to_qemu_small(dest, src, mtu);
        dest += mtu;
        src += mtu;
        len -= mtu;
    }
    ok &= gdb_memcpy_to_qemu_small(dest, src, len);
    return ok;
}

bool gdb_getregs(union gdb_regs* r)
{
    gdb_send(conn, (const uint8_t*)"g", 1);
    size_t   size;
    uint8_t* reply = gdb_recv(conn, &size);

    int      i;
    uint8_t* p = reply;
    uint8_t  c;
    for (i = 0; i < sizeof(union gdb_regs) / sizeof(uint32_t); i++) {
        c           = p[8];
        p[8]        = '\0';
        r->array[i] = gdb_decode_hex_str(p);
        p[8]        = c;
        p += 8;
    }

    free(reply);

    return true;
}

bool gdb_setregs(union gdb_regs* r)
{
    int   len = sizeof(union gdb_regs);
    char* buf = malloc(len * 2 + 128);
    assert(buf != NULL);
    buf[0] = 'G';

    void* src = r;
    int   p   = 1;
    int   i;
    for (i = 0; i < len; i++) {
        p += sprintf(buf + p, "%c%c", hex_encode(((uint8_t*)src)[i] >> 4), hex_encode(((uint8_t*)src)[i] & 0xf));
    }

    gdb_send(conn, (const uint8_t*)buf, strlen(buf));
    free(buf);

    size_t   size;
    uint8_t* reply = gdb_recv(conn, &size);
    bool     ok    = !strcmp((const char*)reply, "OK");
    free(reply);

    return ok;
}

bool gdb_si(void)
{
    char buf[] = "vCont;s:1";
    gdb_send(conn, (const uint8_t*)buf, strlen(buf));
    size_t   size;
    uint8_t* reply = gdb_recv(conn, &size);
    free(reply);
    return true;
}

void gdb_exit(void) { gdb_end(conn); }
<<<<<<< HEAD
=======

#include <ctype.h>

static uint8_t hex_nibble(uint8_t hex) { return isdigit(hex) ? hex - '0' : tolower(hex) - 'a' + 10; }

static bool gdb_memcpy_from_qemu_small(uint32_t src, void* dest, int len)
{
    char buf[128];
    sprintf(buf, "m0x%x,%x", src, len);

    gdb_send(conn, (const uint8_t*)buf, strlen(buf));

    size_t   size;
    uint8_t* reply = gdb_recv(conn, &size);

    if (size > 0 && reply[0] == 'E') {
        free(reply);
        return false;
    }

    for (int i = 0; i < len && i * 2 + 1 < size; i++) {
        uint8_t high        = reply[i * 2];
        uint8_t low         = reply[i * 2 + 1];
        ((uint8_t*)dest)[i] = (hex_nibble(high) << 4) | hex_nibble(low);
    }

    free(reply);
    return true;
}

bool gdb_memcpy_from_qemu(uint32_t src, void* dest, int len)
{
    const int mtu = 1500;
    bool      ok  = true;
    while (len > mtu) {
        ok &= gdb_memcpy_from_qemu_small(src, dest, mtu);
        src += mtu;
        dest = (uint8_t*)dest + mtu;
        len -= mtu;
    }
    ok &= gdb_memcpy_from_qemu_small(src, dest, len);
    return ok;
}
>>>>>>> master
