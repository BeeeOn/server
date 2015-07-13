#include <iostream>
#include <string>
#include <cstring>

#include <curl/curl.h>
#include <jansson.h>

#include "TokenChecker.h"

using namespace std;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

TokenChecker::TokenChecker(string _service) {
    service = _service;

    if (service.compare("google") == 0) {
        str_request = "https://www.googleapis.com/oauth2/v2/userinfo?access_token=";
    }
    else if (service.compare("facebook") == 0) {
        str_request = "https://graph.facebook.com/v2.3/me?access_token=";
    }
    else if (service.compare("vkontakte") == 0) {
        str_request = "https://api.vk.com/method/METHOD_NAME?PARAMETERS&access_token=";
    } 
}

bool TokenChecker::retrieveResponse() {
    CURL *curl;
    CURLcode res;

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

    return true;
}

bool TokenChecker::parseResponse() {
    json_t *root, *data;
    json_error_t error;

    // Converts std::string to char*
    char *response = new char[str_response.size() + 1];
    strcpy(response, str_response.c_str());
    root = json_loads(response, 0, &error);

    delete[] response; // deletes char* version of response

    if (!root) {
        cerr << "bad json" << endl;
        return false;
    }    

    // Parsing data which are 
    data = json_object_get(root, "id");
    if(json_is_string(data))
        id = json_string_value(data);
        
    data = json_object_get(root, "email");
    if(json_is_string(data))
        email = json_string_value(data);

    if (service.compare("google") == 0) {
        
        data = json_object_get(root, "given_name");
        if(json_is_string(data))
            given_name = json_string_value(data);
        
        data = json_object_get(root, "family_name");
        if(json_is_string(data))
            family_name = json_string_value(data);

        data = json_object_get(root, "gender");
        if(json_is_string(data))
            gender = json_string_value(data);        
        
        data = json_object_get(root, "picture");
        if(json_is_string(data))
            picture = json_string_value(data);
    }
    else if (service.compare("facebook") == 0) {
            
        data = json_object_get(root, "first_name");
        if (json_is_string(data))
            given_name = json_string_value(data);

        data = json_object_get(root, "last_name");
        if (json_is_string(data))
            family_name = json_string_value(data);
    }
    else if (service.compare("vkontakte") == 0) {
        ;
    }

    json_decref(root);
    return true;
}

bool TokenChecker::isTokenOkay(string token) {
    str_request.append(token); // Appends token to server API URL

    if (!retrieveResponse())
        return false;

    if (!parseResponse())
        return false;

    return true;
}