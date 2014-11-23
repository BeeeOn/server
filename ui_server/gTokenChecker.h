#ifndef GTOKENCHECKER_H
#define	GTOKENCHECKER_H
#include <string>



using namespace std;

struct googleInfo {
    string id;
    string email;
    bool verified_email;
    string name;
    string given_name;
    string family_name;
    string link;
    string picture;
    string gender;
    string locale;
};

bool isGTokenOk(string gToken, string gid, googleInfo &gInfo);;

#endif	/* GTOKENCHECKER_H */

