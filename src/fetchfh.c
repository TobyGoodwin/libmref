#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gcrypt.h>
#include <gnutls/gnutls.h>

#include "mref.h"

/*** from info gnutls ***/

#define MAX_BUF 1024

extern int tcp_connect(const char *);
extern void tcp_close (int sd);
static int _verify_certificate_callback (gnutls_session_t session);

/*** end info gnutls ***/
enum {
    st_goodbad, st_count, st_data, st_comma, st_toomuch
} state;

mref_err_t mref_fetch_handle(struct mref *m, FILE *h, const char *me) {
    char *mhsh, *rhsh, *store;
    gnutls_session_t sess;
    gnutls_anon_client_credentials_t cred;
    int fd = fileno(h);
    int err, i, store_good;
    unsigned char h_mref[32], h_calc[32];
    gcry_md_hd_t ghd;
    size_t mhsh_len, ns_len, rhsh_len, store_len;

    if (!mref_split(m)) return MREF_ERR_NOT_FIELDS;

    store = mref_field_alloc(m, MREF_FLD_STORE);
    if (!store) return MREF_ERR_NOMEM;
    //printf("store is: %s\n", store);

    mhsh = mref_field_alloc(m, MREF_FLD_MESSAGE_HASH);
    if (!mhsh) return MREF_ERR_NOMEM;
    _mref_b64dec(h_mref, mhsh, mref_field_length(m, MREF_FLD_MESSAGE_HASH));
    //printf("mhsh is: %s\n", mhsh);

    rhsh = mref_field_alloc(m, MREF_FLD_MREF_HASH);
    rhsh_len = mref_field_length(m, MREF_FLD_MREF_HASH);
    if (!rhsh) return MREF_ERR_NOMEM;
    //printf("rhsh is: %s\n", rhsh);

    /*
       err = gnutls_global_init();
       if (err != GNUTLS_E_SUCCESS) return MREF_ERR_TLS(err);
       err = gnutls_init(&sess, GNUTLS_CLIENT);
       if (err != GNUTLS_E_SUCCESS) return MREF_ERR_TLS(err);
       err = gnutls_credentials_set(sess, GNUTLS_CRD_ANON, &cred);
       if (err != GNUTLS_E_SUCCESS) return MREF_ERR_TLS(err);
       */

    /*** from info gnutls ***/

    {
        int ret, sd;
        gnutls_session_t session;
        char buffer[MAX_BUF + 1];
        const char *err;

        /* Initialize TLS session
        */
        gnutls_init (&session, GNUTLS_CLIENT);

        gnutls_session_set_ptr (session, (void *) store);

        gnutls_server_name_set (session, GNUTLS_NAME_DNS, store, strlen(store));

        /* Use default priorities */
        ret = gnutls_priority_set_direct (session, "NORMAL", &err);
        if (ret < 0)
        {
            if (ret == GNUTLS_E_INVALID_REQUEST)
            {
                fprintf (stderr, "Syntax error at: %s\n", err);
            }
            exit (1);
        }

        /* put the x509 credentials to the current session
        */
        gnutls_credentials_set(session, GNUTLS_CRD_CERTIFICATE, mref_cred());

        /* connect to the peer
        */
        sd = tcp_connect(store);
        if (sd == -1) {
            gnutls_deinit (session);
            return MREF_ERR_CONNECT_FAIL;
        }

        gnutls_transport_set_int (session, sd);
        gnutls_handshake_set_timeout (session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

        /* Perform the TLS handshake
        */
        do
            ret = gnutls_handshake (session);
        while (ret < 0 && gnutls_error_is_fatal (ret) == 0);

        if (ret < 0) {
            tcp_close(sd);
            gnutls_deinit(session);
            return MREF_ERR_TLS(ret);
        }

        gnutls_record_send (session, rhsh, rhsh_len);
        //fprintf(stderr, "sent rhsh, %d bytes\n", rhsh_len);

        mref_trace_headers(m, h, sd, me);

        state = st_goodbad;
        ns_len = 0;
        gcry_md_open(&ghd, GCRY_MD_SHA256, 0);
        if (!ghd) return MREF_ERR_GCRYPT;

        while ((ret = gnutls_record_recv (session, buffer, MAX_BUF)) > 0) {
            int i;
            for (i = 0; i < ret; ++i) {

                switch (state) {
                    case st_goodbad:
                        store_good = buffer[i] == '+';
                        ++state;
                        break;
                    case st_count:
                        if (buffer[i] >= '0' && buffer[i] <= '9')
                            ns_len = ns_len * 10 + buffer[i] - '0';
                        else if (buffer[i] == ':') ++state;
                        else return MREF_ERR_STORE_PROTO;
                        break;
                    case st_data:
                        if (store_good) fputc(buffer[i], h);
                        else fputc(buffer[i], stderr);
                        gcry_md_write(ghd, buffer + i, 1);
                        if (--ns_len == 0) ++ state;
                        break;
                    case st_comma:
                        if (buffer[i] == ',') break;
                    case st_toomuch:
                        return MREF_ERR_STORE_PROTO;
                }
            }
        }
        if (ret < 0) {
           if (gnutls_error_is_fatal(ret)) {
               fprintf (stderr, "*** Error: %s\n", gnutls_strerror (ret));
           } else {
               fprintf (stderr, "*** Warning: %s\n", gnutls_strerror (ret));
               gnutls_bye (session, GNUTLS_SHUT_RDWR);
           }
           tcp_close(sd);
           gnutls_deinit (session);
           return MREF_ERR_TLS(ret);
        }

        tcp_close(sd);
        gnutls_deinit (session);

        memcpy(h_calc, gcry_md_read(ghd, 0), 32);
        gcry_md_close(ghd);

        if (memcmp(h_mref, h_calc, 32) != 0) return MREF_ERR_BAD_MSG_HASH;

        if (fflush(h) != 0) return MREF_ERR_SYS;
        if (fsync(fd) != 0) return MREF_ERR_SYS;
        fclose(h); /* cannot fail */

        return 0;
    }
}
