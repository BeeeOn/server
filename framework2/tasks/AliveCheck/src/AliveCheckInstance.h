/* 
 * File:   AliveCheckInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKINSTANCE_H
#define ALIVECHECKINSTANCE_H

#include <string>
#include <memory>

#include "../../../src/TimedTaskInstance.h"

class AliveCheckInstance: public TimedTaskInstance
{
public:
    AliveCheckInstance();
    
    virtual ~AliveCheckInstance();
    
    void run() override;
    
private:

    void executeRefreshQuery();
};
#endif /* ALIVECHECKINSTANCE_H */

