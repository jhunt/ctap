#include "ctap.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CTAP_STACK_SIZE 32

#undef skip
#undef todo
#undef should_fail
#undef pass
#undef fail
#undef ok
#undef tests

static struct {
	int tests;                  /* number of tests so far */
	int expect;                 /* number of tests to expect */
	int fail;                   /* failures (diagnostic use only) */

	int x;                      /* used for for loop trickery */
	int evaled;                 /* is this a controlled exit? */

	pid_t pid;                  /* pid who started the test */
	FILE *stdout;               /* private stdout channel */

	struct {
		int   type;
		char *msg;
	} stack[CTAP_STACK_SIZE];   /* stack of SKIP / TODO */
	int i;                      /* stack index */
} CTAP;

static int eval_test_run(void)
{
	CTAP.evaled = 1;
	if (CTAP.expect >= 0 && CTAP.expect != CTAP.tests) {
		fprintf(CTAP.stdout, "1..%d\n", CTAP.expect);
		ctap_diag(stderr, "Looks like you planned %i tests but ran %i.",
				CTAP.expect, CTAP.tests);
		return 1;
	} else {
		fprintf(CTAP.stdout, "1..%d\n", CTAP.tests);
	}
	if (CTAP.fail > 0) {
		ctap_diag(stderr, "Looks like you failed %d test%s of %d.",
			CTAP.fail, (CTAP.fail == 1 ? "" : "s"), CTAP.tests);
	}
	return CTAP.fail ? 1 : 0;
}

static char* _vstr(const char *fmt, va_list ap1)
{
	char *msg;
	va_list ap2;
	va_copy(ap2, ap1);

	if (!fmt) return NULL;
	size_t len = snprintf(msg, 0, fmt, ap2) + 1;

	msg = malloc((len +  1) * sizeof(char));
	if (!msg) {
		perror("malloc failed");
		exit(1);
	}
	vsnprintf(msg, len, fmt, ap1);
	va_end(ap1);
	return msg;
}

static int _assert(int ok, int autodiag, const char *file, unsigned long line, char *msg)
{
	CTAP.tests++;
	switch (CTAP.stack[CTAP.i].type) {
	case CTAP_SKIP:
		fprintf(CTAP.stdout, "ok %i # skip %s\n", CTAP.tests,
				CTAP.stack[CTAP.i].msg);
		free(msg);
		return 0;

	case CTAP_TODO:
		fprintf(CTAP.stdout, "%sok %i%s%s # TODO %s\n",
				(ok ? "" : "not "), CTAP.tests,
				(msg ? " - " : ""), (msg ? msg   : ""),
				CTAP.stack[CTAP.i].msg);
		if (!ok && autodiag) {
			ctap_diag(CTAP.stdout, "  Failed (TODO) test '%s'", msg);
			if (file) ctap_diag(CTAP.stdout, "  at %s line %d.", file, line);
		}
		free(msg);
		return 0;

	case CTAP_NORM:
	default:
		if (!ok) CTAP.fail++;
		fprintf(CTAP.stdout, "%sok %i%s%s\n",
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
	_exit(eval_test_run());
}

void plan(int n)
{
	/* remove buffering from stdout / stderr */
	setvbuf(CTAP.stdout, NULL, _IONBF, 0);
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
	exit(eval_test_run());
}

#define vmsg(fmt,x) va_list ap; va_start(ap,fmt); char *x = _vstr(fmt, ap); va_end(ap)

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

	nul = open("/dev/null", O_WRONLY);
	if (nul < 0)
		ctap_bail("failed to open /dev/null: %s", strerror(errno));

	CTAP.stdout = fdopen(out, "w");
	if (!CTAP.stdout)
		ctap_bail("failed to fdopen stdout: %s", strerror(errno));

	if (dup2(nul, 1) < 0)
		ctap_bail("failed to redirect stdout to /dev/null: %s", strerror(errno));

	no_plan();
	ctap_tests();
	done_testing();
} // LCOV_EXCL_LINE
