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

#include <jansson.h>
#include "../ui_logger/Logger.h"

/*
typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;
*/
// For this example, we'll be testing on openssl.org
#define SERVER  "www.googleapis.com"
#define PORT 443


// Write function for curl_easy_setopt CURLOPT_WRITEFUNCTION.
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool isGoogleTokenOkayCURL(string gToken, googleInfo &gInfo) {
    CURL *curl;
    CURLcode res;
    
    long http_code = 0; // Stores http response code.
    string str_response; // std::string version of response
    string str_request = "https://www.googleapis.com/oauth2/v2/userinfo?access_token="; // std::string version of request
    str_request.append(gToken);

    // Converts std::string to char*
    char *request = new char[str_request.size() + 1];
    strcpy (request, str_request.c_str());

    curl_global_init(CURL_GLOBAL_DEFAULT);
 
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, request);
        
        delete[] request; // deletes *char version of request

        // Sets curl to write to string.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        
        // Writes input data to string.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_response);

        // Perform the request, res will get the return code.
        res = curl_easy_perform(curl);

        // Check for errors.
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            return false;        
        }
        // Gets response code, if not 200, error.
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code != 200) {
            cerr << "html response: " << http_code << endl;
            return false;
        }
        // Cleanup.
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    // Converts std::string to char*
    char *response = new char[str_response.size() + 1];
    strcpy(response, str_response.c_str());
    
    // JSON part
    json_t *root, *data;
    json_error_t error;

    root = json_loads(response, 0, &error);
    delete[] response; // deletes char* version of response

    if (!root) {
        cerr << "bad json" << endl;
        return false;
    }
        
        data = json_object_get(root, "id");
    if(json_is_string(data))
        gInfo.id = json_string_value(data);

    data = json_object_get(root, "email");
    if(json_is_string(data))
        gInfo.email = json_string_value(data);

    data = json_object_get(root, "verified_email");
    if(json_is_boolean(data))
        gInfo.verified_email = json_boolean(data);

    data = json_object_get(root, "name");
    if(!json_is_string(data))
        gInfo.name = json_string_value(data);

    data = json_object_get(root, "given_name");
    if(json_is_string(data))
        gInfo.given_name = json_string_value(data);

    data = json_object_get(root, "family_name");
    if(json_is_string(data))
        gInfo.family_name = json_string_value(data);

    data = json_object_get(root, "link");
    if(json_is_string(data))
        gInfo.link = json_string_value(data);

    data = json_object_get(root, "picture");
    if(json_is_string(data))
        gInfo.picture = json_string_value(data);

    data = json_object_get(root, "gender");
    if(json_is_string(data))
        gInfo.gender = json_string_value(data);

    data = json_object_get(root, "locale");
    if(json_is_string(data))
        gInfo.locale = json_string_value(data);
        
    json_decref(root);    
    return true;
}