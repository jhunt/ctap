libctap.so - Test Anything Protocol for C
=========================================

**ctap** is an easy way to get Perl-style TAP testing convenience in C.  It
ships as a standalone shared library that you can link to your tests, and a
header file that contains functions and macros for doing things like
assertions, *skip/todo* blocks and dynamic evaluation.

**ctap** stays out of your way, letting you focus on writing tests:

    #include <ctap.h>
    tests {
        ok(1 == 1, "1 does in fact equal 1");
    }

When run, this will output:

    ok 1 - 1 does in fact equal 1
    1..1

This is TAP, so you can use prove and all of its -v option to control
output:


    $ prove t/01-sample
    t/01-sample .. ok
    All tests successful.
    Files=1, Tests=1,  0 wallclock secs ( 0.02 usr +  0.00 sys =  0.02 CPU)
    Result: PASS

    $ prove -v t/01-sample 
    t/01-sample ..
    ok 1 - 1 does in fact equal 1
    1..1
    ok
    All tests successful.
    Files=1, Tests=1,  0 wallclock secs ( 0.01 usr +  0.01 sys =  0.02 CPU)
    Result: PASS

Here's a more complicated example, using some fancier and more well-to-do
assertions like `is_string` and `isnt_null`:

    #include <ctap.h>
    tests {
        char *s = "a string";

        is_null(NULL, "NULL is a null pointer");
        isnt_null(s, "s is not null");
        is_string(s, "a string", "s is 'a string'");
        is_string(s, "empty",    "s is not 'empty'");
    }

And here is the output:

    ok 1 - NULL is a null pointer
    ok 2 - s is not null
    ok 3 - s is 'a string'
    not ok 4 - s is not 'empty'
    #   Failed test 's is not 'empty''
    #   at ./t/01-sample.c line 8.
    #          got: 'a string'
    #     expected: 'empty'
    1..4
    # Looks like you failed 1 test of 4.

Building libctap.so
-------------------

If you've cloned from the upstream git repo, you'll want to bootstrap:

    $ autoreconf -vi

To build, follow the standard process:

    $ ./configure
    $ make
    $ sudo make install

If you want to hack on ctap, don't forget to rebuild all of the autotools
files when you make changes to Makefile.am, configure.ac and friends via
`autoreconf`.

Linking with libctap
--------------------

It couldn't be easier.  Build each test as a standalone executable, and
link them with LDFLAGS of `-lctap`:

    $ gcc -c -o t/01-sample.o t/01-sample.c
    $ gcc -lctap -o t/01-sample.t t/01-sample.o
    $ prove

Assertions, Assertions, Assertions
----------------------------------

### ok(test, "message", ...)

The most basic of assertions, and also one of the most flexible.

    ok(x == 3, "x was three")
    ok(sqrt(y) == 2, "sqrt(y) is 2 (y is %d)", y);

You _can_ write all of your tests using nothing but `ok()`, but you may want
to look at some of the more advanced assertions that give better failure
diagnostics.

### is(a, b, "message", ...)

Assert that strings `a` and `b` are equivalent, even if they point to
different memory regions:

    is(name, "ctap", "Library name should be ctap");

If either value is `NULL`, the assertion will fail, since null strings are
not logically equivalent.

Strings are taken as standard C-style, NULL-terminated strings.  If your
strings are not terminated, you can easily overrun the stack.

### isnt(a, b, "message", ...)

Assert that strings `a` and `b` are different.

    isnt(errstr, "Failed to read data", "errstr isnt a read-fail");

If either value is `NULL`, the assertion will fail.

Strings are taken as standard C-style, NULL-terminated strings.  If your
strings are not terminated, you can easily overrun the stack.

### cmp\_ok(a, "op", b, "message", ...)

Compare two values using arbitrary operators.  This is slightly more useful
than plain old `ok()`, because it will print both values and the operator
via `diag()` when it fails.

    cmp_ok(x(), "!=", y(), "x() and y() are different values");
    cmp_ok(f(g()), "==", g(f()), "f() and g() are composable");

### pass("message", ...)

Unconditionally pass.  The following are equivalent:

    pass("works for me!");
    ok(1, "works for me!");

### fail("message", ...)

Unconditionally fail.  The following are equivalent:

    fail("broken");
    ok(0, "broken");

Diagnostics and Notations
-------------------------

### diag("message", ...)

Prints a diagnostic message.  This message will not interfere with the test
output.

    diag("sleeping for up to %d seconds", timeout);

Running under `prove` (without `-v`) will suppress all output from `diag()`.

### note("message", ...)

Works like `diag()`, except that `prove` will display the message whether in
verbose mode and normal mode (with and without `-v`).

    note("testing %s v%s", PACKAGE, PACKAGE_VERSION);

Skip and Todo Blocks
--------------------

**ctap** supports skip and todo blocks, via the **SKIP** and **TODO**
macros:

    SKIP("not ready for primetime") {
        ok(experimental_function(), "should be ok");
    }

    TODO("api-internals are still under heavy rework") {
        ok(api_internals(), "should work fine");
    }

All tests in a **SKIP** block will still be run, but **ctap** will pretend
as if they had implicitly succeeded.  In a **TODO** block, test can fail but
will not count against the test suite as a normal failure would.

**Note:** these macros are themselves experimental, and their interfaces may
change in future versions of libctap.  Do _not_ rely on them in real test
suites.
