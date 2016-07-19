#include <iostream>
#include <vector>
#include <postgresql/soci-postgresql.h>
#include <soci.h>
#include "sqlCommands.h"
#include "loger.h"

struct tping
{
	long long gateway_id; /**< id brany*/
	time_t ping_sent; /**< sent timestamp*/
	time_t ping_received; /**< received timestamp*/
};

class PingService
{
	const std::string m_Name = "PingService"; /**< name of class*/
	static PingService *single;

	PingService()
	{
	}
public:
	static soci::session *m_connection;
	Loger *m_log; /**<  Logger used to log */
	static std::vector<tping> m_gateways;

	static PingService *getInstance();
	void createConn(std::string connStr);
	void addRecord(long long int gatewayID, time_t timestamp);
	void senderCheckRecord(long long int gatewayID);
	void receiverCheckRecord(long long int gatewayID, time_t  timestamp);
	void setStatus(long long int gatewayID, std::string status);

	bool SQLUpdateGatewayStatus(long long int gatewayID, std::string status);
	bool SQLUpdateAllGatewaysStatus(std::string status);

	~PingService()
	{
	}
};
