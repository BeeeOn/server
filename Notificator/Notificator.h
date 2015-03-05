#ifndef NOTIFICATOR_H
#define NOTIFICATOR_H

#include "Notification.h"

const static char* GCM_SERVER = "https://android.googleapis.com/gcm/send";

const static char* HEADER_API_KEY = "Authorization: key=AIzaSyDKkEYdTwSkDT2P-AFpl9BN0ZsadEUkWX0";
const static char* HEADER_CONTENT_TYPE = "Content-Type: application/json";
const static char* HEADER_PROJECT_ID = "project_id:72175563561";
const static char* HEADER_ACCEPT = "Accept:";

class Notificator {
public:
    static int sendNotification(Notification &message);
protected:
private:
    static size_t writeToString(void *ptr, size_t size, size_t count, void *stream);

    Notificator() {
    };
    //        virtual ~Notificator();
};

#endif // NOTIFICATOR_H
