#ifndef CTAP_H
#define CTAP_H

#define CTAP_VERSION_MAJOR 2
#define CTAP_VERSION_MINOR 0
#define CTAP_VERSION_REV   0

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

void ctap_tests(void);
#define TESTS   void ctap_tests(void)
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


/*************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CTAP_STACK_SIZE 32

static struct {
	int tests;                  /* number of tests so far */
	int expect;                 /* number of tests to expect */
	int fail;                   /* failures (diagnostic use only) */

	int x;                      /* used for for loop trickery */
	int evaled;                 /* is this a controlled exit? */

	pid_t pid;                  /* pid who started the test */
	FILE *priv_stdout;          /* private stdout channel */

	struct {
		int   type;
		char *msg;
	} stack[CTAP_STACK_SIZE];   /* stack of SKIP / TODO */
	int i;                      /* stack index */
} CTAP;

static int _ctap_eval_test_run(void)
{
	CTAP.evaled = 1;
	if (CTAP.expect >= 0 && CTAP.expect != CTAP.tests) {
		fprintf(CTAP.priv_stdout, "1..%d\n", CTAP.expect);
		ctap_diag(stderr, "Looks like you planned %i tests but ran %i.",
				CTAP.expect, CTAP.tests);
		return 1;
	} else {
		fprintf(CTAP.priv_stdout, "1..%d\n", CTAP.tests);
	}
	if (CTAP.fail > 0) {
		ctap_diag(stderr, "Looks like you failed %d test%s of %d.",
			CTAP.fail, (CTAP.fail == 1 ? "" : "s"), CTAP.tests);
	}
	return CTAP.fail ? 1 : 0;
}

static char* _ctap_vstr(const char *fmt, va_list ap1)
{
	if (!fmt) return NULL;

	char *msg;
	va_list ap2;
	va_copy(ap2, ap1);
	size_t len = vsnprintf(NULL, 0, fmt, ap2) + 1;
	va_end(ap2);

	msg = malloc((len +  1) * sizeof(char));
	if (!msg) {
		perror("malloc failed");
		exit(1);
	}
	vsnprintf(msg, len, fmt, ap1);
	return msg;
}

static int _assert(int ok, int autodiag, const char *file, unsigned long line, char *msg)
{
	CTAP.tests++;
	switch (CTAP.stack[CTAP.i].type) {
	case CTAP_SKIP:
		fprintf(CTAP.priv_stdout, "ok %i # skip %s\n", CTAP.tests,
				CTAP.stack[CTAP.i].msg);
		free(msg);
		return 0;

	case CTAP_TODO:
		fprintf(CTAP.priv_stdout, "%sok %i%s%s # TODO %s\n",
				(ok ? "" : "not "), CTAP.tests,
				(msg ? " - " : ""), (msg ? msg   : ""),
				CTAP.stack[CTAP.i].msg);
		if (!ok && autodiag) {
			ctap_diag(CTAP.priv_stdout, "  Failed (TODO) test '%s'", msg);
			if (file) ctap_diag(CTAP.priv_stdout, "  at %s line %d.", file, line);
		}
		free(msg);
		return 0;

	case CTAP_NORM:
	default:
		if (!ok) CTAP.fail++;
		fprintf(CTAP.priv_stdout, "%sok %i%s%s\n",
				(ok ? "" : "not "), CTAP.tests,
				(msg ? " - " : ""), (msg ? msg : ""));
		if (!ok && autodiag) {
			ctap_diag(stderr, "  Failed test '%s'", msg);
			if (file) ctap_diag(stderr, "  at %s line %d.", file, line);
		}
		free(msg);
		break;
	}

	return ok;
}

/*******************************************************************/

int ctapX(void) { return CTAP.x = 0; }
int ctapY(void) { return CTAP.x < 1; }
int ctapZ(void) { return CTAP.x = 1; }

int ctap_diag(FILE *io, const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	fprintf(io, "# ");
	vfprintf(io, msg, ap);
	fprintf(io, "\n");

	va_end(ap);

	return 0;
}

void ctap_bail(const char *msg, ...)
{
	ctap_diag(stderr, "bailing out: %s", msg);
	CTAP.evaled = 1;
	exit(1);
}

void ctap_push(int type, const char *msg)
{
	CTAP.i++;
	if (CTAP.i >= CTAP_STACK_SIZE) {
		ctap_bail("ctap stack depth exceeded (too many skip/todo calls!)");
	}

	CTAP.stack[CTAP.i].type = type;
	CTAP.stack[CTAP.i].msg  = strdup(msg);

	if (!CTAP.stack[CTAP.i].msg) {
		ctap_bail("Failed to strdup skip/todo message");
	}
}

void ctap_atexit(void)
{
	if (CTAP.evaled != 0) return;
	if (CTAP.pid != getpid()) return;
	_exit(_ctap_eval_test_run());
}

void plan(int n)
{
	/* remove buffering from stdout / stderr */
	setvbuf(CTAP.priv_stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	CTAP.pid = getpid();
	CTAP.fail = 0;
	CTAP.tests = 0;
	CTAP.expect = n;

	memset(CTAP.stack, 0, sizeof(CTAP.stack));
	CTAP.i = 0;
	CTAP.stack[0].type = CTAP_NORM;

	// register the atexit handler
	atexit(ctap_atexit);
}

void no_plan(void)
{
	plan(-1);
}

void done_testing(void)
{
	exit(_ctap_eval_test_run());
}

#define vmsg(fmt,x) va_list ap; va_start(ap,fmt); char *x = _ctap_vstr(fmt, ap); va_end(ap)

int ctap_assert(int ok, int autodiag, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	return _assert(ok, autodiag, file, line, msg);
}

void ctap_cmp_ok(int a, const char *op, int b, const char *file, unsigned long line, const char *fmt, ...)
{
	int k = strcmp(op, "||") == 0 ? (a || b)
	      : strcmp(op, "&&") == 0 ? (a && b)
	      : strcmp(op, "|")  == 0 ? (a |  b)
	      : strcmp(op, "^")  == 0 ? (a ^  b)
	      : strcmp(op, "&")  == 0 ? (a &  b)
	      : strcmp(op, "==") == 0 ? (a == b)
	      : strcmp(op, "!=") == 0 ? (a != b)
	      : strcmp(op, "<")  == 0 ? (a <  b)
	      : strcmp(op, "<=") == 0 ? (a <= b)
	      : strcmp(op, ">")  == 0 ? (a >  b)
	      : strcmp(op, ">=") == 0 ? (a >= b)
	      : strcmp(op, "<<") == 0 ? (a << b)
	      : strcmp(op, ">>") == 0 ? (a >> b)
	      : strcmp(op, "+")  == 0 ? (a +  b)
	      : strcmp(op, "-")  == 0 ? (a -  b)
	      : strcmp(op, "/")  == 0 ? (a /  b)
	      : strcmp(op, "%")  == 0 ? (a %  b)
	      : diag("unknown operator: '%s'", op);

	vmsg(fmt, msg);
	if (!_assert(k, 0, file, line, msg)) {
		diag("   %d %s %d failed", a, op, b);
	}
}

void ctap_pop(void)
{
	if (CTAP.i > 0) {
		free(CTAP.stack[CTAP.i].msg);
		CTAP.stack[CTAP.i].type = CTAP_NORM;
		CTAP.stack[CTAP.i].msg  = NULL;
		CTAP.i--;
	}
}

void ctap_eq_u64(uint64_t x, uint64_t y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x == y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %llu (%x)", x, x);
		ctap_diag(stderr, "    expected: %llu (%x)", y, y);
	}
}
void ctap_ne_u64(uint64_t x, uint64_t y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x != y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %llu (%x)", x, x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}
void ctap_eq_i64(int64_t x, int64_t y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x == y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %lli (%x)", x, x);
		ctap_diag(stderr, "    expected: %lli (%x)", y, y);
	}
}
void ctap_ne_i64(int64_t x, int64_t y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x != y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %lli (%x)", x, x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}

void ctap_eq_ptr(const void *x, const void *y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x == y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %p", x);
		ctap_diag(stderr, "    expected: %p", y);
	}
}
void ctap_ne_ptr(const void *x, const void *y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(x != y, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: %p", x);
		ctap_diag(stderr, "    expected: <anything else>", y);
	}
}

void ctap_eq_string(const char *x, const char *y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!x && !y) {
		_assert(0, 0, file, line, msg);
		return;
	}

	if (x == y) {
		_assert(1, 0, file, line, msg);
		return;
	}

	if (!_assert(x && y && strcmp(x, y) == 0, 1, file, line, msg)) {
		ctap_diag(stderr, "         got: '%s'", x);
		ctap_diag(stderr, "    expected: '%s'", y);
	}
}
void ctap_ne_string(const char *x, const char *y, const char *file, unsigned long line, const char *fmt, ...)
{
	vmsg(fmt, msg);
	if (!_assert(!(x && y && strcmp(x, y) == 0), 1, file, line, msg)) {
		ctap_diag(stderr, "         got: '%s'", x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}

int main(int argc, char **argv)
{
	int nul, out;

	out = dup(1); /* keep a hold of stdout, for our own purposes */
	if (out < 0)
		ctap_bail("failed to dup stdout: %s", strerror(errno));

#ifdef _WIN32
	char nullpath[] = "NUL";
#else
	char nullpath[] = "/dev/null";
#endif
	nul = open(nullpath, O_WRONLY);
	if (nul < 0)
		ctap_bail("failed to open %s: %s", nullpath, strerror(errno));

	CTAP.priv_stdout = fdopen(out, "w");
	if (!CTAP.priv_stdout)
		ctap_bail("failed to fdopen stdout: %s", strerror(errno));

	if (dup2(nul, 1) < 0)
		ctap_bail("failed to redirect stdout to %s: %s", nullpath, strerror(errno));

	no_plan();
	ctap_tests();
	done_testing();
	return 0;
} // LCOV_EXCL_LINE
