#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

/* tcp.c */
int tcp_connect (const char *);
void tcp_close (int sd);

/* Connects to the peer and returns a socket
 * descriptor.
 */
int tcp_connect(const char *server) {
  const char *port = "983";
  int err, sd;
  struct addrinfo hints;
  struct addrinfo *addr, *ap;
  struct sockaddr_in sa;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG | AI_NUMERICSERV;
  hints.ai_protocol = 0;          /* Any protocol */
  hints.ai_canonname = 0;
  hints.ai_addr = 0;
  hints.ai_next = 0;

  err = getaddrinfo(server, port, &hints, &addr);
  printf("err is %d\n", err);
  if (err != 0) {
      printf("err string => %s\n", gai_strerror(err));
      exit(1);
  }

  /*
  memset (&sa, '\0', sizeof (sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons (atoi (PORT));
  memcpy(sa.sin_addr, h.
  inet_pton (AF_INET, SERVER, &sa.sin_addr);
  */

  /* connects to server
   */
  for (ap = addr; ap; ap = ap->ai_next) {
      sd = socket(ap->ai_family, ap->ai_socktype, ap->ai_protocol);
      if (sd == -1) continue;
    if (connect(sd, ap->ai_addr, ap->ai_addrlen) != -1)
        break;
    close(sd);
  }

  if (!ap) {
      fprintf (stderr, "Connect error\n");
      exit (1);
  }

  freeaddrinfo(addr);

  return sd;
}

/* closes the given socket descriptor.
 */
extern void
tcp_close (int sd)
{
  shutdown (sd, SHUT_RDWR);     /* no more receptions */
  close (sd);
}
