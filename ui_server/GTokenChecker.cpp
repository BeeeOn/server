#include <string>
#include "gTokenChecker.h"
#include <iostream>
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <fstream>

#include "openssl/ssl.h"
#include "openssl/bio.h"
#include "openssl/err.h"

#include "stdio.h"
#include "string.h"

using namespace std;

        
        #include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;

// For this example, we'll be testing on openssl.org
#define SERVER  "www.googleapis.com"
#define PORT 443

// Establish a regular tcp connection
int tcpConnect ()
{
  int error, handle;
  struct hostent *host;
  struct sockaddr_in server;

  host = gethostbyname (SERVER);
  handle = socket (AF_INET, SOCK_STREAM, 0);
  if (handle == -1)
    {
      perror ("Socket");
      handle = 0;
    }
  else
    {
      server.sin_family = AF_INET;
      server.sin_port = htons (PORT);
      server.sin_addr = *((struct in_addr *) host->h_addr);
      bzero (&(server.sin_zero), 8);

      error = connect (handle, (struct sockaddr *) &server,
                       sizeof (struct sockaddr));
      if (error == -1)
        {
          perror ("Connect");
          handle = 0;
        }
    }

  return handle;
}

// Establish a connection using an SSL layer
connection *sslConnect (void)
{
  connection *c;

  c = (connection*) malloc (sizeof (connection));
  c->sslHandle = NULL;
  c->sslContext = NULL;

  c->socket = tcpConnect ();
  if (c->socket)
    {
      // Register the error strings for libcrypto & libssl
      SSL_load_error_strings ();
      // Register the available ciphers and digests
      SSL_library_init ();

      // New context saying we are a client, and using SSL 2 or 3
      c->sslContext = SSL_CTX_new (SSLv23_client_method ());
      if (c->sslContext == NULL)
        ERR_print_errors_fp (stderr);

      // Create an SSL struct for the connection
      c->sslHandle = SSL_new (c->sslContext);
      if (c->sslHandle == NULL)
        ERR_print_errors_fp (stderr);

      // Connect the SSL struct to our connection
      if (!SSL_set_fd (c->sslHandle, c->socket))
        ERR_print_errors_fp (stderr);

      // Initiate SSL handshake
      if (SSL_connect (c->sslHandle) != 1)
        ERR_print_errors_fp (stderr);
    }
  else
    {
      perror ("Connect failed");
    }

  return c;
}

// Disconnect & free connection struct
void sslDisconnect (connection *c)
{
  if (c->socket)
    close (c->socket);
  if (c->sslHandle)
    {
      SSL_shutdown (c->sslHandle);
      SSL_free (c->sslHandle);
    }
  if (c->sslContext)
    SSL_CTX_free (c->sslContext);

  free (c);
}

// Read all available text from the connection
char *sslRead (connection *c)
{
  const int readSize = 10240;
  char *rc = NULL;
  int received, count = 0;
  char buffer[10240];

  if (c)
    {
      while (1)
        {
          if (!rc)
            rc = (char*)malloc (readSize * sizeof (char) + 1);
          else
            rc = (char*)realloc (rc, (count + 1) *
                          readSize * sizeof (char) + 1);

          received = SSL_read (c->sslHandle, buffer, readSize);
          buffer[received] = '\0';

          if (received > 0)
            strcat (rc, buffer);

          if (received < readSize)
            break;
          count++;
        }
    }

  return rc;
}

// Write text to the connection
void sslWrite (connection *c, char *text)
{
  if (c)
    SSL_write (c->sslHandle, text, strlen (text));
}


bool isGTokenOk(string gToken, string mail){
        return true;
#ifdef S_DEBUG
        return true;
#endif
       connection *c;
  char *response;
  char gRequest[1024]="GET https://www.googleapis.com/oauth2/v2/userinfo?access_token=";
  strcat(gRequest, gToken.c_str());
  strcat(gRequest, " \r\n\r\n");
  c = sslConnect ();

  cout<<"GTokenCheck: "<<gRequest<<endl;
  //sslWrite (c, "GET https://www.googleapis.com/oauth2/v2/userinfo?access_token=ya29.1.AADtN_WaZGtm1fzqqtOnvCRckz9z1oLM5LwxkppcrewnkxsF8HxHQYVdOY0Vag \r\n\r\n");
  sslWrite(c, gRequest);
  response = sslRead (c);
  //TODO potřeba udělat přes json parser!
/*
  string json = response;
  
  std::size_t found = json.find("email");
  if (found!=std::string::npos)
    std::cout << "first 'needle' found at: " << found << '\n';
  */
  
  printf ("%s\n", response);

  sslDisconnect (c);
  free (response);

  return 1;   
}
/*
 {
  "family_name": "Vampola", 
  "name": "Pavel Vampola", 
  "picture": "https://lh3.googleusercontent.com/-XdUIqdMkCWA/AAAAAAAAAAI/AAAAAAAAAAA/4252rscbv5M/photo.jpg", 
  "locale": "cs", 
  "gender": "other", 
  "email": "pvampola@gmail.com", 
  "link": "https://plus.google.com/112433523823668947901", 
  "given_name": "Pavel", 
  "id": "112433523823668947901", 
  "verified_email": true
}
 * 
 {
 "error": {
  "errors": [
   {
    "domain": "global",
    "reason": "authError",
    "message": "Invalid Credentials",
    "locationType": "header",
    "location": "Authorization"
   }
  ],
  "code": 401,
  "message": "Invalid Credentials"
 }
}
 */