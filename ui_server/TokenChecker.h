#ifndef FTOKENCHECKER_H
#define	FTOKENCHECKER_H
#include <string>

using namespace std;

class TokenChecker {
private:
    string service; // Stores which service API will be used.

    long http_code; // Stores http response code. 

    string str_request; // URL address to API server.
    string str_response; // std::string version of response.

    char *response; // Response in JSON format from service API server.

    // Gets response from API server in JSON format.
    bool retrieveResponse();
    
    // Parses received JSON file. 
    bool parseResponse();
public:
    // Informations about a user.
    string id;
    string email;
    string given_name;
    string family_name;
    string gender;
    string picture;

    TokenChecker(string _service);

    // Checks if token is okay, retrieves response and parses it.
    bool isTokenOkay(string token);
};


#endif