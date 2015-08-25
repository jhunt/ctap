#ifndef CTAP_H
#define CTAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <unistd.h>

#define CTAP_NORM 0x0000
#define CTAP_TODO 0x0001
#define CTAP_SKIP 0x0002

int  ctap_diag(FILE *io, const char *msg, ...);
void ctap_bail(const char *msg, ...);
int  ctap_assert(int ok, int autodiag, const char *file, unsigned long line, const char *msg, ...);
void ctap_pop(void);
void ctap_push(int type, const char *msg);
void ctap_bailed_early(void);

void plan(int n);
void no_plan(void);
void done_testing(void);

int ctapX(void);
int ctapY(void);
int ctapZ(void);

void (*CTAP_TESTS)(void) = NULL;
#define TESTS extern void (*CTAP_TESTS)(void); void ctap_tests(void); CTAP_TESTS = ctap_tests; void ctap_tests(void)
//#define TESTS   void ctap_tests(void)
#define subtest for (ctapX(); ctapY(); ctapZ())

/*************************************************************************/

void ctap_eq_string(const char *x, const char *y, const char *file, unsigned long line, const char *msg, ...);
void ctap_ne_string(const char *x, const char *y, const char *file, unsigned long line, const char *msg, ...);

void ctap_eq_u64(uint64_t x, uint64_t y, const char *file, unsigned long line, const char *msg, ...);
void ctap_ne_u64(uint64_t x, uint64_t y, const char *file, unsigned long line, const char *msg, ...);
void ctap_eq_i64(int64_t x, int64_t y, const char *file, unsigned long line, const char *msg, ...);
void ctap_ne_i64(int64_t x, int64_t y, const char *file, unsigned long line, const char *msg, ...);

void ctap_eq_ptr(const void *x, const void *y, const char *file, unsigned long line, const char *msg, ...);
void ctap_ne_ptr(const void *x, const void *y, const char *file, unsigned long line, const char *msg, ...);

void ctap_cmp_ok(int a, const char *op, int b, const char *file, unsigned long line, const char *msg, ...);

/*************************************************************************/

#define ok(t, ...) ctap_assert(!!(t), 1, __FILE__, __LINE__, __VA_ARGS__)

#define   is(a, b, ...) ctap_eq_string((a), (b), __FILE__, __LINE__, __VA_ARGS__)
#define isnt(a, b, ...) ctap_ne_string((a), (b), __FILE__, __LINE__, __VA_ARGS__)

// like
//#define   like(a, re, ...) ctap_eq_regex((a), (re), __FILE__, __LINE__, __VA_ARGS__)
// unlike
//#define unlike(a, re, ...) ctap_ne_regex((a), (re), __FILE__, __LINE__, __VA_ARGS__)

#define cmp_ok(a, op, b, ...) ctap_cmp_ok((a), (op), (b), __FILE__, __LINE__, __VA_ARGS__)

#define pass(...)  ctap_assert(1, 0, __FILE__, __LINE__, __VA_ARGS__)
#define fail(...)  ctap_assert(0, 1, __FILE__, __LINE__, __VA_ARGS__)

/*************************************************************************/

#define   is_null(p, ...) ctap_assert((p)==NULL, 1, __FILE__, __LINE__, __VA_ARGS__)
#define isnt_null(p, ...) ctap_assert((p)!=NULL, 1, __FILE__, __LINE__, __VA_ARGS__)

/* is_string/isnt_string are aliases for is/isnt */
#define   is_string(x, y, ...) ctap_eq_string((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_string(x, y, ...) ctap_ne_string((x), (y), __FILE__, __LINE__, __VA_ARGS__)

#define   is_unsigned(x, y, ...) ctap_eq_u64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_unsigned(x, y, ...) ctap_ne_u64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define   is_uint(x, y, ...)     ctap_eq_u64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_uint(x, y, ...)     ctap_ne_u64((x), (y), __FILE__, __LINE__, __VA_ARGS__)

#define   is_signed(x, y, ...)   ctap_eq_i64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_signed(x, y, ...)   ctap_ne_i64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define   is_int(x, y, ...)      ctap_eq_i64((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_int(x, y, ...)      ctap_ne_i64((x), (y), __FILE__, __LINE__, __VA_ARGS__)


#define   is_pointer(x, y, ...)  ctap_eq_ptr((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_pointer(x, y, ...)  ctap_ne_ptr((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define   is_ptr(x, y, ...)      ctap_eq_ptr((x), (y), __FILE__, __LINE__, __VA_ARGS__)
#define isnt_ptr(x, y, ...)      ctap_ne_ptr((x), (y), __FILE__, __LINE__, __VA_ARGS__)

/*************************************************************************/

#define SKIP(why) for (ctapX(), ctap_push(CTAP_SKIP, why); ctapY(); ctap_pop(), ctapZ())
#define TODO(why) for (ctapX(), ctap_push(CTAP_TODO, why); ctapY(); ctap_pop(), ctapZ())
#define BAIL_OUT(msg) ctap_bail(msg)

#define note(...) ctap_diag(stdout, __VA_ARGS__)
#define diag(...) ctap_diag(stderr, __VA_ARGS__)

#endif
