TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh

TESTS = valid.test
EXTRA_DIST = $(TESTS)
LDADD = ../src/libmref.la $(DL_LIBS) $(LIBGCRYPT_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/src $(LIBGCRYPT_CFLAGS)

check_PROGRAMS = check-valid
