#include <iostream>
#include <time.h>
#include <mutex>
#include "ping.h"

using namespace soci;

PingService* PingService::single = NULL;
std::vector<tping> PingService::m_gateways;
soci::session *PingService::m_connection;
std::mutex ping_mutex;

PingService *PingService::getInstance()
{
	std::lock_guard<std::mutex> lock(ping_mutex);

	if (PingService::single == nullptr) {
		single = new PingService();
	}

	return single;
}

void PingService::createConn(std::string connStr)
{
	PingService::m_connection = new session(postgresql, connStr);
}

void PingService::addRecord(long long int gateway_id, time_t timestamp)
{
	PingService::m_gateways.push_back({gateway_id, timestamp, 0});
}

void PingService::senderCheckRecord(long long int gateway_id)
{
	std::lock_guard<std::mutex> lock(ping_mutex);

	this->m_log->WriteMessage(TRACE,"Entering " + this->m_Name + "::PingSenderCheckRecord");

	for (size_t i = 0; i < PingService::m_gateways.size(); i++) {
		if (PingService::m_gateways[i].ping_received == 0) {
			this->m_log->WriteMessage(INFO,"PING: Unavailable device: " + std::to_string(gateway_id));
			this->setStatus(gateway_id, "unavailable");
			PingService::m_gateways.erase(PingService::m_gateways.begin() + i);
		}
	}

	this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::PingSenderCheckRecord");
}

void PingService::receiverCheckRecord(long long int gateway_id, time_t timestamp)
{
	std::lock_guard<std::mutex> lock(ping_mutex);

	this->m_log->WriteMessage(TRACE,"Entering " + this->m_Name + "::PingReceiverCheckRecord");

	for (size_t i = 0; i < PingService::m_gateways.size(); i++) {
		if (PingService::m_gateways[i].gateway_id == gateway_id) {
			this->m_log->WriteMessage(INFO,"PING: Device found.");

			if (PingService::m_gateways[i].ping_sent < timestamp) {
				this->m_log->WriteMessage(INFO,"PING: Available device: " + std::to_string(gateway_id));
				this->setStatus(gateway_id, "available");
				PingService::m_gateways.erase(PingService::m_gateways.begin() + i);
			}
		}
	}

	this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::PingReceiverCheckRecord");
}

void PingService::setStatus(long long int gateway_id, std::string status)
{
	this->m_log->WriteMessage(TRACE,"Entering " + this->m_Name + "::PingSetStatus");
	this->m_log->WriteMessage(INFO,"PING: Status: " + status + " ID: " + std::to_string(gateway_id));
	this->SQLUpdateGatewayStatus(gateway_id, status);
	this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::PingSetStatus");
}

bool PingService::SQLUpdateGatewayStatus(long long int gateway_id, std::string status)
{
	this->m_log->WriteMessage(TRACE,"Entering " + this->m_Name + "::SQLUpdateGatewayStatus");
	try
	{
		this->m_log->WriteMessage (INFO, "update gateways set status = " + status + " where gateways = " + std::to_string(gateway_id));
		std::string retRec;

		*PingService::m_connection << SQLQueries::UpdateGatewayStatus,
				into(retRec),
				use(gateway_id, "GATEWAY_ID"),
				use(status, "STATUS");

		this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::SQLUpdateGatewayStatus");
		return true;
	}
	catch(std::exception const &e)
	{
		this->m_log->WriteMessage(ERR, "Error in query : " + m_connection->get_last_query() );
		std::string errorMessage = "Database Error : ";
		errorMessage.append (e.what());
		this->m_log->WriteMessage(ERR,errorMessage );
		this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::SQLUpdateGatewayStatus");
		return false;
	}
}

bool PingService::SQLUpdateAllGatewaysStatus(std::string status)
{
	this->m_log->WriteMessage(TRACE,"Entering " + this->m_Name + "::SQLUpdateAllGatewaysStatus");
	try
	{
		this->m_log->WriteMessage (INFO, "update gateways set status = " + status);
		std::string retRec;

		*PingService::m_connection << SQLQueries::UpdateAllGatewayStatus,
						into(retRec),
						use(status, "STATUS");

		this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::SQLUpdateAllGatewaysStatus");
		return true;
	}
	catch(std::exception const &e)
	{
		this->m_log->WriteMessage(ERR, "Error in query : " + m_connection->get_last_query() );
		std::string errorMessage = "Database Error : ";
		errorMessage.append (e.what());
		this->m_log->WriteMessage(ERR,errorMessage );
		this->m_log->WriteMessage(TRACE,"Exiting " + this->m_Name + "::SQLUpdateAllGatewaysStatus");
		return false;
	}
}
