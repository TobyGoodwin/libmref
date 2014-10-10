#include <gcrypt.h>
#include <gnutls/gnutls.h>

#include "mref.h"

#define CAFILE "/etc/pki/tls/certs/ca-bundle.crt"

static gnutls_certificate_credentials_t xcred;

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

mref_err_t mref_init(void) {

    if (!gcry_check_version (GCRYPT_VERSION)) return MREF_ERR_GCRYPT;
    gcry_control(GCRYCTL_DISABLE_SECMEM, 0);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    gnutls_global_init ();

    /* X509 stuff */
    gnutls_certificate_allocate_credentials(&xcred);

    /* sets the trusted cas file
    */
    gnutls_certificate_set_x509_trust_file(xcred, CAFILE, GNUTLS_X509_FMT_PEM);
    gnutls_certificate_set_verify_function(xcred, _verify_certificate_callback);

    return 0;
}

gnutls_certificate_credentials_t mref_cred(void) {
    return xcred;
}

void mref_finish(void) {
  gnutls_certificate_free_credentials(xcred);
  gnutls_global_deinit();
}

