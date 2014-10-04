#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gnutls/gnutls.h>

#include "mref.h"

    /*** from info gnutls ***/

#define MAX_BUF 1024
#define CAFILE "/etc/pki/tls/certs/ca-bundle.crt"

extern int tcp_connect(const char *);
extern void tcp_close (int sd);
static int _verify_certificate_callback (gnutls_session_t session);

/*** end info gnutls ***/
enum {
    st_goodbad, st_count, st_data, st_comma, st_toomuch
} state;

mref_err_t mref_fetch_handle(struct mref *m, FILE *h) {
    char *mhsh, *rhsh, *store;
    gnutls_session_t sess;
    gnutls_anon_client_credentials_t cred;
    int fd = fileno(h);
    int err, store_good;
    size_t mhsh_len, ns_len, rhsh_len, store_len;

    if (!mref_split(m)) return MREF_ERR_NOT_FIELDS;

    store = mref_field_alloc(m, MREF_FLD_STORE);
    if (!store) return MREF_ERR_NOMEM;
    printf("store is: %s\n", store);

    mhsh = mref_field_alloc(m, MREF_FLD_MESSAGE_HASH);
    if (!mhsh) return MREF_ERR_NOMEM;
    printf("mhsh is: %s\n", mhsh);

    rhsh = mref_field_alloc(m, MREF_FLD_MREF_HASH);
    rhsh_len = mref_field_length(m, MREF_FLD_MREF_HASH);
    if (!rhsh) return MREF_ERR_NOMEM;
    printf("rhsh is: %s\n", rhsh);

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
  gnutls_certificate_credentials_t xcred;

  gnutls_global_init ();

  /* X509 stuff */
  gnutls_certificate_allocate_credentials (&xcred);

  /* sets the trusted cas file
   */
  gnutls_certificate_set_x509_trust_file (xcred, CAFILE, GNUTLS_X509_FMT_PEM);
  gnutls_certificate_set_verify_function (xcred, _verify_certificate_callback);

  /* If client holds a certificate it can be set using the following:
   *
     gnutls_certificate_set_x509_key_file (xcred,
                                           "cert.pem", "key.pem",
                                           GNUTLS_X509_FMT_PEM);
   */

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
  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, xcred);

  gnutls_credentials_set (session, GNUTLS_CRD_CERTIFICATE, xcred);

  /* connect to the peer
   */
  sd = tcp_connect(store);
  if (sd == -1) {
      fprintf(stderr, "connect failed\n");
      goto end;
  }

  gnutls_transport_set_int (session, sd);
  gnutls_handshake_set_timeout (session, GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);

  /* Perform the TLS handshake
   */
  do
    {
      ret = gnutls_handshake (session);
    }
  while (ret < 0 && gnutls_error_is_fatal (ret) == 0);

  if (ret < 0)
    {
      fprintf (stderr, "*** Handshake failed\n");
      gnutls_perror (ret);
      goto end;
    }
  else
    {
      char* desc;

      desc = gnutls_session_get_desc(session);
      printf ("- Session info: %s\n", desc);
      gnutls_free(desc);
    }

  gnutls_record_send (session, rhsh, rhsh_len);
fprintf(stderr, "sent rhsh, %d bytes\n", rhsh_len);

    state = st_goodbad;
    ns_len = 0;

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
                    if (--ns_len == 0) ++ state;
                    break;
                case st_comma:
                    if (buffer[i] == ',') break;
                case st_toomuch:
                    return MREF_ERR_STORE_PROTO;
            }
        }
    }
  if (ret == 0)
    {
      printf ("- Peer has closed the TLS connection\n");
      goto end;
    }
  else if (ret < 0 && gnutls_error_is_fatal (ret) == 0)
    {
      fprintf (stderr, "*** Warning: %s\n", gnutls_strerror (ret));
    }
  else if (ret < 0)
    {
      fprintf (stderr, "*** Error: %s\n", gnutls_strerror (ret));
      goto end;
    }

  gnutls_bye (session, GNUTLS_SHUT_RDWR);

end:

  tcp_close (sd);

  gnutls_deinit (session);

  gnutls_certificate_free_credentials (xcred);

  gnutls_global_deinit ();

}

/*** end info gnutls ***/


    if (fflush(h) != 0) return MREF_ERR_SYS;
    if (fsync(fd) != 0) return MREF_ERR_SYS;
    fclose(h); /* cannot fail */
    return 0;
}

/*** start info gnutls ***/
/* This function will verify the peer's certificate, and check
 * if the hostname matches, as well as the activation, expiration dates.
 */
static int
_verify_certificate_callback (gnutls_session_t session)
{
  unsigned int status;
  int ret, type;
  const char *hostname;
  gnutls_datum_t out;

return 0;

  /* read hostname */
  hostname = gnutls_session_get_ptr (session);
  /* read hostname */
  hostname = gnutls_session_get_ptr (session);

  /* This verification function uses the trusted CAs in the credentials
   * structure. So you must have installed one or more CA certificates.
   */
  ret = gnutls_certificate_verify_peers3 (session, hostname, &status);
  if (ret < 0)
    {
      printf ("Error\n");
      return GNUTLS_E_CERTIFICATE_ERROR;
    }

  type = gnutls_certificate_type_get (session);

  ret = gnutls_certificate_verification_status_print( status, type, &out, 0);
  if (ret < 0)
    {
      printf ("Error\n");
      return GNUTLS_E_CERTIFICATE_ERROR;
    }

  printf ("%s", out.data);

  gnutls_free(out.data);

  if (status != 0) /* Certificate is not trusted */
      return GNUTLS_E_CERTIFICATE_ERROR;

  /* notify gnutls to continue handshake normally */
  return 0;
}


    /*** end info gnutls ***/
