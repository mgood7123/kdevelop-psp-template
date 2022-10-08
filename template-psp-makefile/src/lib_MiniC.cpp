#include <MiniC.h>

void MiniC::snprnt_cb(void* ctx, int ch) {
    if(ch == 0x200 || ch == 0x201)
        return;

    xprintf_ctx* ctx_p = (xprintf_ctx*)ctx;

    if(ctx_p->cpylen < *(ctx_p->len)){
        ctx_p->buf[ctx_p->cpylen] = ch;
        ctx_p->cpylen++;
    }
}

int MiniC::vsnprintf(char *buf, size_t n, const char *fmt, va_list ap) {
    xprintf_ctx ctx = {buf, (const size_t *)&n, 0};

    prnt(snprnt_cb, (void *)&ctx, fmt, ap);

    if(ctx.cpylen >= *(ctx.len)) {
        ctx.buf[*(ctx.len) - 1] = '\0';
    } else {
        ctx.buf[ctx.cpylen] = '\0';
    }

    return ctx.cpylen;
}

void MiniC::log(const char *fmt, ...) {
    va_list list;
    char data[2048];

    va_start(list, fmt);
    vsnprintf(data, 2048, fmt, list);
    va_end(list);

    //printf("%s\n", data);

    //_log_file(data);
    //_log_psplink(data);
}
