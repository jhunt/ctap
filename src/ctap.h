#ifndef CTAP_INCLUDED
#define CTAP_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#define CTAP_STACK_SIZE 32
#define CTAP_NORM 0
#define CTAP_TODO 1
#define CTAP_SKIP 2

static struct {
	int tests;                  /* number of tests so far */
	int expect;                 /* number of tests to expect */
	int fail;                   /* failures (diagnostic use only) */
	int flip;                   /* pass = fail; fail = pass */

	int x;                      /* used for for loop trickery */
	int exited;                 /* is this a controlled exit? */

	struct {
		int   type;
		char *msg;
	} stack[CTAP_STACK_SIZE];   /* stack of SKIP / TODO */
	int i;                      /* stack index */
} CTAP;

static void _ctap_diag(FILE *io, const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	fprintf(io, "# ");
	vfprintf(io, msg, ap);
	fprintf(io, "\n");

	va_end(ap);
}

static void _ctap_bail(const char *msg)
{
	_ctap_diag(stderr, "# bailing out: %s\n", msg);
	CTAP.exited = 1;
	exit(1);
}

static int _ctap_assert(int ok, const char *msg, int autodiag, const char *file, unsigned long line)
{
	CTAP.tests++;
	switch (CTAP.stack[CTAP.i].type) {
	case CTAP_SKIP:
		fprintf(stdout, "ok %i # skip %s\n", CTAP.tests,
				CTAP.stack[CTAP.i].msg);
		return 0;
		break;

	case CTAP_TODO:
		fprintf(stdout, "%sok %i%s%s # TODO %s\n",
				(ok ? "" : "not "), CTAP.tests,
				(msg ? " - " : ""), (msg ? msg   : ""),
				CTAP.stack[CTAP.i].msg);
		if (!ok) {
			_ctap_diag(stdout, "  Failed (TODO) test '%s'", msg);
			if (file) _ctap_diag(stdout, "  at %s line %d.", file, line);
		}
		return 0;
		break;

	case CTAP_NORM:
	default:
		if (!ok) CTAP.fail++;
		fprintf(stdout, "%sok %i%s%s\n",
				(ok ? "" : "not "), CTAP.tests,
				(msg ? " - " : ""), (msg ? msg : ""));
		if (!ok && autodiag) {
			_ctap_diag(stderr, "  Failed test '%s'", msg);
			if (file) _ctap_diag(stderr, "  at %s line %d.", file, line);
		}
		break;
	}

	return ok;
}

static void _ctap_stack_pop(void)
{
	if (CTAP.i > 0) {
		free(CTAP.stack[CTAP.i].msg);
		CTAP.stack[CTAP.i].type = CTAP_NORM;
		CTAP.stack[CTAP.i].msg  = NULL;
		CTAP.i--;
	}
}

static void _ctap_stack_push(int type, const char *msg)
{
	CTAP.i++;
	if (CTAP.i >= CTAP_STACK_SIZE) {
		_ctap_bail("ctap stack depth exceeded (too many skip/todo calls!)");
	}

	CTAP.stack[CTAP.i].type = type;
	CTAP.stack[CTAP.i].msg  = strdup(msg);

	if (!CTAP.stack[CTAP.i].msg) {
		_ctap_bail("Failed to strdup skip/todo message");
	}
}

/************************************************************/

static void _ctap_bailed_early(void)
{
	if (CTAP.exited != 0) return;
	_ctap_diag(stderr, "exiting, and done_testing() not seen\n");
}

static void plan(int n)
{
	/* remove buffering from stdout / stderr */
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	CTAP.tests = 0;
	CTAP.expect = n;

	memset(CTAP.stack, 0, sizeof(CTAP.stack));
	CTAP.i = 0;
	CTAP.stack[0].type = CTAP_NORM;

	// register the bailed_early handler
	atexit(_ctap_bailed_early);
}
#define no_plan() plan(-1)

static void done_testing(void)
{
	fprintf(stdout, "1..%d\n", CTAP.tests);
	if (CTAP.fail > 0) {
		_ctap_diag(stderr, "Looks like you failed %d test%s of %d.",
			CTAP.fail, (CTAP.fail == 1 ? "" : "s"), CTAP.tests);
	}
	CTAP.exited = 1;
	exit(CTAP.fail ? 1 : 0);
}
static void skip(const char *why) { _ctap_stack_push(CTAP_SKIP, why); }
static void todo(const char *why) { _ctap_stack_push(CTAP_TODO, why); }
static void resume() { _ctap_stack_pop(); }

#define pass(msg)  _ctap_assert(1,   (msg), 0, __FILE__, __LINE__)
#define fail(msg)  _ctap_assert(0,   (msg), 1, __FILE__, __LINE__)
#define ok(t, msg) _ctap_assert((t), (msg), 1, __FILE__, __LINE__)

#define ok_pointer(p, msg) _ctap_assert((p)!=NULL, (msg), 1, __FILE__, __LINE__)
#define ok_null(p, msg)    _ctap_assert((p)==NULL, (msg), 1, __FILE__, __LINE__)

#define isnt_null(p, msg) _ctap_assert((p)!=NULL, (msg), 1, __FILE__, __LINE__)
#define is_null(p, msg)   _ctap_assert((p)==NULL, (msg), 1, __FILE__, __LINE__)

static void _ctap_eq_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!_ctap_assert(x == y, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: %llu (%x)", x, x);
		_ctap_diag(stderr, "    expected: %llu (%x)", y, y);
	}
}
static void _ctap_ne_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!_ctap_assert(x != y, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: %llu (%x)", x, x);
		_ctap_diag(stderr, "    expected: <anything else>");
	}
}
static void _ctap_eq_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!_ctap_assert(x == y, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: %lli (%x)", x, x);
		_ctap_diag(stderr, "    expected: %lli (%x)", y, y);
	}
}
static void _ctap_ne_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!_ctap_assert(x != y, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: %lli (%x)", x, x);
		_ctap_diag(stderr, "    expected: <anything else>");
	}
}
#define   is_unsigned(x,y,msg) _ctap_eq_u64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_unsigned(x,y,msg) _ctap_ne_u64((x), (y), (msg), __FILE__, __LINE__)
#define   is_uint(x,y,msg)     _ctap_eq_u64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_uint(x,y,msg)     _ctap_ne_u64((x), (y), (msg), __FILE__, __LINE__)

#define   is_signed(x,y,msg)   _ctap_eq_i64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_signed(x,y,msg)   _ctap_ne_i64((x), (y), (msg), __FILE__, __LINE__)
#define   is_int(x,y,msg)      _ctap_eq_i64((x), (y), (msg), __FILE__, __LINE__)
#define isnt_int(x,y,msg)      _ctap_ne_i64((x), (y), (msg), __FILE__, __LINE__)

static void _ctap_eq_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line)
{
	if (x == y) {
		_ctap_assert(1, msg, 0, file, line);
		return;
	}

	if (!_ctap_assert(x && y && strcmp(x, y) == 0, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: '%s'", x);
		_ctap_diag(stderr, "    expected: '%s'", y);
	}
}
static void _ctap_ne_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line)
{
	if (!_ctap_assert(!x || !y || strcmp(x, y) != 0, msg, 1, file, line)) {
		_ctap_diag(stderr, "         got: '%s'", x);
		_ctap_diag(stderr, "    expected: <anything else>");
	}
}
#define   is_string(x,y,msg) _ctap_eq_string((x), (y), (msg), __FILE__, __LINE__)
#define isnt_string(x,y,msg) _ctap_ne_string((x), (y), (msg), __FILE__, __LINE__)


#define bail(msg) _ctap_bail(msg)

#define tests   int main(void)
#define subtest for (CTAP.x=0; CTAP.x<1; CTAP.x++)

#define diag(msg, ...) _ctap_diag(stderr, msg, __VA_ARGS__)

static void sys(const char *cmd)
{
	int rc = system(cmd);
}

#endif
