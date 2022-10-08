#ifndef MINI_C_H
#define MINI_C_H

#include <stdlib.h>
#include <stdarg.h>
#include <cstring>

extern "C" {
	#include <pspsysclib.h>
}

class MiniC {
public:
    typedef struct {
        char *buf;
        const size_t *len;
        size_t cpylen;
    } xprintf_ctx;

    static void snprnt_cb(void* ctx, int ch);
    static int vsnprintf(char *buf, size_t n, const char *fmt, va_list ap);
    static void log(const char *fmt, ...);
};

#endif
