/* 
 * File:   DataMessage.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 28. March 2016
 */

#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H

#include <map>
 
enum class MODULE_STATUS {
    AVAILABLE, UNAVAILABLE
};

struct DataMessage {
    /** At least 8 bytes. Decimal(20,0) in database. */
    long long gateway_id;
    /** At least 4 bytes. Decimal(10) in database. */
    long device_euid;
    /** Unix timestamp. */ 
    unsigned int time;
    /** Map storing informations about modules.  */
    std::map<int /*module_id*/, std::pair<MODULE_STATUS /*status of module*/, double /*value of module*/>> modules;
};

#endif /* DATAMESSAGE_H */
