#ifndef CTAP_H
#define CTAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#define CTAP_NORM 0x0000
#define CTAP_TODO 0x0001
#define CTAP_SKIP 0x0002

void ctap_diag(FILE *io, const char *msg, ...);
void ctap_bail(const char *msg);
int  ctap_assert(int ok, const char *msg, int autodiag, const char *file, unsigned long line);
void ctap_pop(void);
void ctap_push(int type, const char *msg);
void ctap_bailed_early(void);
void plan(int n);
void no_plan(void);
void done_testing(void);

int ctap_test(int,char**);

#define skip(why) for (ctapX(), ctap_push(CTAP_SKIP, why); ctapY(); ctap_pop(), ctapZ())
#define todo(why) for (ctapX(), ctap_push(CTAP_TODO, why); ctapY(); ctap_pop(), ctapZ())

#define pass(msg)  ctap_assert(1,   (msg), 0, __FILE__, __LINE__)
#define fail(msg)  ctap_assert(0,   (msg), 1, __FILE__, __LINE__)
#define ok(t, msg) ctap_assert((t), (msg), 1, __FILE__, __LINE__)

#define ok_pointer(p, msg) ctap_assert((p)!=NULL, (msg), 1, __FILE__, __LINE__)
#define ok_null(p, msg)    ctap_assert((p)==NULL, (msg), 1, __FILE__, __LINE__)

#define isnt_null(p, msg) ctap_assert((p)!=NULL, (msg), 1, __FILE__, __LINE__)
#define is_null(p, msg)   ctap_assert((p)==NULL, (msg), 1, __FILE__, __LINE__)

void ctap_eq_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line);
void ctap_ne_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line);
void ctap_eq_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line);
void ctap_ne_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line);

#define   is_unsigned(x,y,msg) ctap_eq_u64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_unsigned(x,y,msg) ctap_ne_u64((x), (y), (msg), __FILE__, __LINE__)
#define   is_uint(x,y,msg)     ctap_eq_u64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_uint(x,y,msg)     ctap_ne_u64((x), (y), (msg), __FILE__, __LINE__)

#define   is_signed(x,y,msg)   ctap_eq_i64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_signed(x,y,msg)   ctap_ne_i64((x), (y), (msg), __FILE__, __LINE__)
#define   is_int(x,y,msg)      ctap_eq_i64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_int(x,y,msg)      ctap_ne_i64((x), (y), (msg), __FILE__, __LINE__)

void ctap_eq_ptr(const void *x, const void *y, const char *msg, const char *file, unsigned long line);
void ctap_ne_ptr(const void *x, const void *y, const char *msg, const char *file, unsigned long line);

#define   is_pointer(x,y,msg)  ctap_eq_ptr((x), (y), (msg), __FILE__, __LINE__)
#define isnt_pointer(x,y,msg)  ctap_ne_ptr((x), (y), (msg), __FILE__, __LINE__)
#define   is_ptr(x,y,msg)      ctap_eq_ptr((x), (y), (msg), __FILE__, __LINE__)
#define isnt_ptr(x,y,msg)      ctap_ne_ptr((x), (y), (msg), __FILE__, __LINE__)

void ctap_eq_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line);
void ctap_ne_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line);

#define   is_string(x,y,msg) ctap_eq_string((x), (y), (msg), __FILE__, __LINE__)
#define isnt_string(x,y,msg) ctap_ne_string((x), (y), (msg), __FILE__, __LINE__)

#define bail(msg) ctap_bail(msg)
#define diag(...) ctap_diag(stderr, __VA_ARGS__)

#define tests   int ctap_tests(void)
#define subtest for (ctapX(); ctapY(); ctapZ())

#endif
