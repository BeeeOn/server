#include "Notificator.h"

#include <iostream>
#include <curl/curl.h>

vector<string> Notificator::sendGcm(string json) {
    CURL *curl;
    //CURLcode res;

    struct curl_slist *headers = NULL; // init to NULL is important 
    headers = curl_slist_append(headers, HEADER_CONTENT_TYPE);
    headers = curl_slist_append(headers, HEADER_API_KEY);
    headers = curl_slist_append(headers, HEADER_PROJECT_ID);
    headers = curl_slist_append(headers, HEADER_ACCEPT);
    curl = curl_easy_init();

    string response;

    cout << json << endl;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, GCM_SERVER);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json.length());   
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    
    vector<string> gcmDelete;
    gcmDelete.clear();

    return gcmDelete;
}

size_t writeToString(void *ptr, size_t size, size_t count, void *stream) {
    ((string*) stream)->append((char*) ptr, 0, size * count);
    return size*count;
}
