TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh

TESTS = fetch.test search.test valid.test
EXTRA_DIST = $(TESTS)
LDADD = ../src/libmref.la $(DL_LIBS) $(LIBGCRYPT_LIBS) $(LIBGNUTLS_LIBS)

AM_CPPFLAGS = -I$(top_srcdir)/src $(LIBGCRYPT_CFLAGS) $(LIBGNUTLS_CFLAGS)

check_PROGRAMS = check-fetch check-search check-valid
