#include "ctap.h"

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
	int exited;                 /* is this a controlled exit? */

	struct {
		int   type;
		char *msg;
	} stack[CTAP_STACK_SIZE];   /* stack of SKIP / TODO */
	int i;                      /* stack index */
} CTAP;

int ctapX() { return CTAP.x = 0; }
int ctapY() { return CTAP.x < 1; }
int ctapZ() { return CTAP.x = 1; }

void ctap_diag(FILE *io, const char *msg, ...)
{
	va_list ap;
	va_start(ap, msg);

	fprintf(io, "# ");
	vfprintf(io, msg, ap);
	fprintf(io, "\n");

	va_end(ap);
}

void ctap_bail(const char *msg)
{
	ctap_diag(stderr, "# bailing out: %s", msg);
	CTAP.exited = 1;
	exit(1);
}

int ctap_assert(int ok, const char *msg, int autodiag, const char *file, unsigned long line)
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
			ctap_diag(stdout, "  Failed (TODO) test '%s'", msg);
			if (file) ctap_diag(stdout, "  at %s line %d.", file, line);
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
			ctap_diag(stderr, "  Failed test '%s'", msg);
			if (file) ctap_diag(stderr, "  at %s line %d.", file, line);
		}
		break;
	}

	return ok;
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
	if (CTAP.exited != 0) return;
	if (CTAP.expect == CTAP.tests) {
		fprintf(stdout, "1..%d\n", CTAP.expect);
		if (CTAP.fail > 0) {
			ctap_diag(stderr, "Looks like you failed %d test%s of %d.",
				CTAP.fail, (CTAP.fail == 1 ? "" : "s"), CTAP.tests);
		}
		_exit(CTAP.fail ? 1 : 0);
	}
	ctap_diag(stderr, "exiting, and done_testing() not seen!");
}

void plan(int n)
{
	/* remove buffering from stdout / stderr */
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

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
	fprintf(stdout, "1..%d\n", CTAP.tests);
	if (CTAP.fail > 0) {
		ctap_diag(stderr, "Looks like you failed %d test%s of %d.",
			CTAP.fail, (CTAP.fail == 1 ? "" : "s"), CTAP.tests);
	}
	CTAP.exited = 1;
	exit(CTAP.fail ? 1 : 0);
}

void ctap_eq_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x == y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %llu (%x)", x, x);
		ctap_diag(stderr, "    expected: %llu (%x)", y, y);
	}
}
void ctap_ne_u64(uint64_t x, uint64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x != y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %llu (%x)", x, x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}
void ctap_eq_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x == y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %lli (%x)", x, x);
		ctap_diag(stderr, "    expected: %lli (%x)", y, y);
	}
}
void ctap_ne_i64(int64_t x, int64_t y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x != y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %lli (%x)", x, x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}

void ctap_eq_ptr(const void *x, const void *y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x == y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %p", x);
		ctap_diag(stderr, "    expected: %p", y);
	}
}
void ctap_ne_ptr(const void *x, const void *y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(x != y, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: %p", x);
		ctap_diag(stderr, "    expected: <anything else>", y);
	}
}

void ctap_eq_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line)
{
	if (!x && !y) {
		ctap_assert(0, msg, 0, file, line);
		return;
	}

	if (x == y) {
		ctap_assert(1, msg, 0, file, line);
		return;
	}

	if (!ctap_assert(x && y && strcmp(x, y) == 0, msg, 1, file, line)) {
		ctap_diag(stderr, "         got: '%s'", x);
		ctap_diag(stderr, "    expected: '%s'", y);
	}
}
void ctap_ne_string(const char *x, const char *y, const char *msg, const char *file, unsigned long line)
{
	if (!ctap_assert(!(x && y && strcmp(x, y) == 0), msg, 1, file, line)) {
		ctap_diag(stderr, "         got: '%s'", x);
		ctap_diag(stderr, "    expected: <anything else>");
	}
}

int main(int argc, char **argv)
{
	ctap_tests();
	done_testing();
}
