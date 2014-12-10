#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

using namespace std;

class Notification
{
    public:
        virtual ~Notification() {};
        virtual string getJson() = 0;
//        virtual string getDbXml() = 0;
    protected:
        Notification(){};
    private:
};

#endif
