#include <iostream>
#include <string>
#include <memory>

#include <asio.h>
#include <iostream>

#include "WatchdogNotif.h"
#include "DeleteNotif.h"
#include "AchievementNotif.h"
#include "UriNotif.h"

#include <vector>
#include <memory>

int main()
{   
    soci::session sql("port = '5432' dbname = 'home7' user = 'uiserver7' password = '1234' connect_timeout = '3'");

    std::vector<std::string> service_reference_ids;
    sql << "SELECT service_reference_ids FROM push_notification_service WHERE user_id = 9509",
           soci::into(service_reference_ids);

    std::cout << "Service ids of user 9509" << std::endl;
    for (std::string id : service_reference_ids) {
        std::cout << id << std::endl;
    }
    //std::vector<std::string> service_reference_ids;
    //service_reference_ids.push_back("f6MPWjr144I:APA91bE4_2j9YKMGD5iLiB5jn8UkcGWEfv3zOqcYWScYc7FflpNXuK9tW78hrfvwMdsBV0O97Kk_98ML2ml7tk3L63aZs2eIEApHRsoO5nRNUvJbgXOn0-WJ4COrdgawkn0TT0TwVR7R");
    
    int user_id = 9509;
    int notif_id = 84564;
    long timestamp = 1458737981;
    std::string message = "Testovaci URI notifikace.";
    std::string uri = "www.google.cz";

    std::shared_ptr<Notification> advert = new UriNotif(user_id, notif_id, timestamp, message, uri);

    std::vector<std::string> non_valid_ids;
    non_valid_ids = advert->sendGcm(&service_reference_ids);

    std::cout << "Non valid ids: " << std::endl;
    for (std::string id : non_valid_ids) {
        std::cout << id << std::endl;
    }

    delete(advert);

    //ids.push_back("APA91bF-u3AZ3bNl6jehq19oEhbVFGEaA_4x3jlzMtKwHzlakRcE9K_H_Rk8YuNSjp1K62dz-_sL09iQjlH8Z3JTsxbg1VbV_Gch0YhTuOg3U1bCib7pk4Xg4rHPeUrICiQ74RIB3qXxTjmTCeLBZor_Tr3pLuATUw");
    /*    
    Notification *notif = new WatchdogNotif(
        9509,                    // user ID (MrMaidx -> Martin Novak)
        9999,                  // message (notification) ID
        timestamp,        // timestamp (ms)
        "Test notifikace watchdog.",   // message
        4321,                  // adadpter ID
        "",               // sensor ID
        10,                  // sensor type
        3);                   // rule ID
    
    notif->sendGcm(&service_reference_ids);
    */
 /*
    sleep(5);   

    Notification *notif2 = new DeleteNotif(
        8,
        2,
        45,
        0);

    notif2->sendGcm(&ids);
   */ 
    /*
    if (notif->saveToDb()) {
      cout << endl << "########   SAVING TO DATABASE ############" << endl;
      int userId = notif->getUserId();
      long timestamp = notif->getTime();
      int notificationId = notif->getId();
      int level = notif->getLevel();
      string name = notif->getName();
      cout << notif->getDbXml() << endl;
    }

 
    Notification *notif2 = new AchievementNotif(
        8,                  // user ID
        123,                // message (notification) ID
        1418220573673,      // timestamp (ms)
        1                   // achievement ID
        );

    notif2->sendGcm(&ids);

    delete(notif);
  
    return 0;
}
