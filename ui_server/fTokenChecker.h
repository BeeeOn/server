#ifndef FTOKENCHECKER_H
#define	FTOKENCHECKER_H
#include <string>


struct facebookInfo {
    std::string id;
    std::string email; 
    std::string first_name;
    std::string last_name;
    std::string link;
    std::string name;
    std::string gender;
    int timezone;
    std::string updated_time;
    bool verified;
};

bool isFTokenOkay(std::string fToken, facebookInfo &fInfo);

#endif