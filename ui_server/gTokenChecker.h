#ifndef GTOKENCHECKER_H
#define	GTOKENCHECKER_H
#include <string>


struct googleInfo {
    std::string id;
    std::string email;
    bool verified_email;
    std::string name;
    std::string given_name;
    std::string family_name;
    std::string link;
    std::string picture;
    std::string gender;
    std::string locale;
};

bool isGTokenOk(std::string gToken, googleInfo &gInfo);;

#endif	/* GTOKENCHECKER_H */

