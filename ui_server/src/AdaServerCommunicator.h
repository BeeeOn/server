#ifndef ADASERVERCOMMUNICATOR_H
#define ADASERVERCOMMUNICATOR_H

#include "DAO/DAO.h"
#include <string>

class AdaServerCommunicator 
{
    public:
        AdaServerCommunicator(int port);
        virtual ~AdaServerCommunicator();
        
        int sendSetState(gatewayId_64b gatewayId, deviceId_64b deviceId, int deviceType, std::string newValue);
        int sendUnregisterDevice(gatewayId_64b gatewayId, deviceId_64b deviceId);
        int sendGatewayStartListen(gatewayId_64b gatewayId);
        int sendGatewayStartSearch(gatewayId_64b gatewayId, std::string IP, std::string deviceEuid);
    private:
        int _port;
};

#endif /* ADASERVERCOMMUNICATOR_H */

