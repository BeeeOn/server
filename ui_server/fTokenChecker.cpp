#include <iostream>
#include <string>
#include <cstring>

#include <curl/curl.h>
#include <jansson.h>

#include "fTokenChecker.h"

using namespace std;

// Write function for curl_easy_setopt CURLOPT_WRITEFUNCTION.
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool isFTokenOkay(string fToken, facebookInfo &fInfo) {
    CURL *curl;
    CURLcode res;
    
    long http_code = 0; // Stores http response code.
    string str_response; // std::string version of response
    string str_request = "https://graph.facebook.com/v2.3/me?access_token="; // std::string version of request
    str_request.append(fToken);

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
    if (json_is_string(data))
        fInfo.id = json_string_value(data);
        
    data = json_object_get(root, "email");
    if (json_is_string(data))
        fInfo.email = json_string_value(data);
        
    data = json_object_get(root, "first_name");
    if (json_is_string(data))
        fInfo.first_name = json_string_value(data);

    data = json_object_get(root, "last_name");
    if (json_is_string(data))
        fInfo.last_name = json_string_value(data);
    
    data = json_object_get(root, "gender");
    if (json_is_string(data))
        fInfo.gender = json_string_value(data);

    data = json_object_get(root, "link");
    if (json_is_string(data))
        fInfo.link = json_string_value(data);

    data = json_object_get(root, "name");
    if (json_is_string(data))
        fInfo.name = json_string_value(data);
        
    data = json_object_get(root, "timezone");
    if (json_is_integer(data))
        fInfo.timezone = json_integer_value(data);
        
    data = json_object_get(root, "updated_time");
    if (json_is_string(data))
        fInfo.updated_time = json_string_value(data);

    data = json_object_get(root, "verified");
    if (json_is_boolean(data))
        fInfo.verified = json_boolean(data);
        
    json_decref(root);    
    return true;
}