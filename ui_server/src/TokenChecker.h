#ifndef XXTOKENCHECKER_H
#define	XXTOKENCHECKER_H
#include <string>


class TokenChecker {
private:
    std::string service; // Stores which service API will be used.

    long http_code; // Stores http response code. 

    std::string str_request; // URL address to API server.
    std::string str_response; // std::string version of response.

    char *response; // Response in JSON format from service API server.

    // Gets response from API server in JSON format.
    bool retrieveResponse();
    
    // Parses received JSON file. 
    bool parseResponse();
public:
    // Informations about a user.
    std::string id;
    std::string email;
    std::string given_name;
    std::string family_name;
    std::string gender;
    std::string picture;

    TokenChecker(std::string _service);

    // Checks if token is okay, retrieves response and parses it.
    bool isTokenOkay(std::string token);
};


#endif