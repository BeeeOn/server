#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <vector>

using namespace std;

class Notification
{
  
    public:
        virtual ~Notification() {};
        virtual string getJson() = 0;
        virtual string getDbXml() = 0;
        virtual string getId() = 0;
        virtual int getLevel() = 0;
        virtual vector<string> send() = 0;
    
    protected:
        Notification(){};

    private:
};

#endif
