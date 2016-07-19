/*
 * File:   AliveCheckInstance.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#include "AliveCheckInstance.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <memory>
#include <random>

#include <soci.h>

#include "../../../../Notificator/UriNotif.h"

#include "../../../src/DatabaseInterface.h"
#include "../../../src/GatewayInterface.h"
#include "../../../src/Logger.h"
#include "../../../src/TaskManager.h"

AliveCheckInstance::AliveCheckInstance(long instance_id,
									   std::weak_ptr<TaskManager> owning_manager):
	TaskInstance(instance_id, owning_manager),
	TimedTaskInstance(instance_id, owning_manager)
{
	// Plan this instance randomly between zero and one minute.
	// This is because of reloading of instances from database,
	// because they are loaded at the same time.
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 60);
	planActivationAfterSeconds(distr(eng));
}

AliveCheckInstance::~AliveCheckInstance()
{
}

void AliveCheckInstance::run(std::chrono::system_clock::time_point activation_time)
{
	logger.LOGFILE("alive_check", "INFO") << "AliveCheck instance with instance_id: "
			<< m_instance_id << " has been run." << std::endl;

	planActivationAfterSeconds(30);

	runAliveCheck();
}

void AliveCheckInstance::runAliveCheck()
{
	int device_id;
	double device_euid;
	long long measured_at;
	int refresh;

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	// Find out from configuration if instance should send notification.
	short send_notif;
	*sql << "SELECT send_notif FROM task_alive_check WHERE instance_id = :instance_id",
			 soci::into(send_notif),
			 soci::use(m_instance_id, "instance_id");

	// Get identificator of gateway from configuration.
	long long gateway_id = getGatewayId();

	// Check if gateway is online.
	// This checking is temporary disabled, because ping functionality
	// on ADA Server Sender doesn't work correctly.
	//checkGatewayStatus(gateway_id, send_notif);

	// Get information about devices.
	soci::rowset<soci::row> rows = (sql->prepare << "SELECT device_type, device_euid, measured_at, refresh FROM device WHERE gateway_id = :gateway_id",
									soci::use(gateway_id, "gateway_id"));

	for (soci::rowset<soci::row>::const_iterator it = rows.begin(); it != rows.end(); ++it) {

		soci::row const& row = *it;
		// Get values from rows.
		device_id = row.get<int>(0);

		// Defined devices with these device ids (type in database means device id anywhere else)
		// don't contain any sensors, so they don't send
		// any sensoric data to server. That means this method of checking if they are
		// available will not work, and they would always be marked as unavailable.
		if (device_id == 7 || device_id == 8 ||
			device_id == 9 || device_id == 17) {
			continue;
		}
		device_euid = row.get<double>(1);
		measured_at = row.get<long long>(2);
		refresh = row.get<int>(3);

		// Get timestamp of current time.
		int now_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		std::string status;
		*sql << "SELECT status FROM device WHERE device_euid = :device_euid",
				soci::into(status),
				soci::use(static_cast<long>(device_euid), "device_euid");

		// Three times refresh was chosen as the best value to test device availability.
		if ((measured_at + (3 * refresh)) < now_timestamp) {

			if (status != "unavailable") {

				*sql << "UPDATE device SET status = 'unavailable'::device_status WHERE device_euid = :device_euid",
						soci::use(static_cast<long>(device_euid), "device_euid");

				// Send notification
				if (send_notif == 1) {
					// Only type in which soci can get device_euid from database is double,
					// so it must be casted.
					std::string notification("Zařízení s euid: ");
					notification += std::to_string(static_cast<long>(device_euid));
					notification += " je nedostupné.";

					sendUnavailableNotification(notification);
				}
				logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id << " - Device with device_euid: "
					   << static_cast<long>(device_euid) << " is now unavailable." << std::endl;
			}

		}
		else {
			if (status != "available") {

				*sql << "UPDATE device SET status = 'available'::device_status WHERE device_euid = :device_euid",
					soci::use(device_euid, "device_euid");

				logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id
						<< " - Device with device_euid: " << static_cast<long>(device_euid) << " is now available." << std::endl;
			}

		}
	}
}

long long AliveCheckInstance::getGatewayId()
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	// Get identificator of gateway from configuration of instance.
	soci::indicator ind_gateway_id;
	long long gateway_id;
	*sql << "SELECT gateway_id FROM task_alive_check WHERE instance_id = :instance_id",
			soci::into(gateway_id, ind_gateway_id),
			soci::use(m_instance_id, "instance_id");

	if (ind_gateway_id != soci::i_ok) {
		logger.LOGFILE("alive_check", "ERROR") << "Instance: " << m_instance_id << " tried to get id of gateway "
				  << " which doesn't exist: gateway_id: " << gateway_id << std::endl;
	}
	return gateway_id;
}

void AliveCheckInstance::checkGatewayStatus(long long gateway_id, short send_notif)
{
	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();
	GatewayInterface gi;

	std::string status;
	*sql << "SELECT status FROM gateway WHERE gateway_id = :gateway_id",
			soci::into(status),
			soci::use(gateway_id, "gateway_id");

	if (!gi.pingGateway(gateway_id)) {

		if (status != "unavailable") {

			// Gateway is unavailable.
			*sql << "UPDATE gateway SET status = 'unavailable'::gateway_status WHERE gateway_id = :gateway_id",
					 soci::use(gateway_id, "gateway_id");

			if (send_notif == 1) {
				// Send notification.
				std::string notification("Brána s id: ");
				notification += std::to_string(gateway_id);
				notification += " je nedostupná.";

				sendUnavailableNotification(notification);

				logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id << " - Gateway with gateway_id: "
				<< gateway_id << " is now unavailable." << std::endl;
			}
		}
	}
	else {
		if (status != "available") {

			*sql << "UPDATE gateway SET status = 'available'::gateway_status WHERE gateway_id = :gateway_id",
					 soci::use(gateway_id, "gateway_id");

			logger.LOGFILE("alive_check", "INFO") << "Instance: " << m_instance_id
					<< " - Gateway with gateway_id: " << gateway_id << " is now available." << std::endl;
		}
	}
}

void AliveCheckInstance::sendUnavailableNotification(std::string notification)
{
	int user_id;
	// Get gateway_id from database.
	long long gateway_id = getGatewayId();

	SessionSharedPtr sql = DatabaseInterface::getInstance()->makeNewSession();

	// Find all users who have access to this gateway.
	soci::rowset<soci::row> user_rows = (sql->prepare << "SELECT user_id FROM user_gateway WHERE gateway_id = :gateway_id",
														  soci::use(gateway_id, "gateway_id"));

	for (soci::rowset<soci::row>::const_iterator user_it = user_rows.begin(); user_it != user_rows.end(); ++user_it) {

		soci::row const& user_row = *user_it;
		//Get id of a first user.
		user_id = user_row.get<int>(0);

		// Find all service_reference_ids binded with user.
		soci::rowset<soci::row> sri_rows = (sql->prepare << "SELECT service_reference_id "
					"FROM push_notification_service WHERE user_id = :user_id",
					 soci::use(user_id, "user_id"));

		std::vector<std::string> sr_ids;
		for (soci::rowset<soci::row>::const_iterator sri_it = sri_rows.begin(); sri_it != sri_rows.end(); ++sri_it) {

			soci::row const& sri_row = *sri_it;
			//Get all service_reference_ids.
			sr_ids.push_back(sri_row.get<std::string>(0));
		}

		long now_timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		// URI notif is just placeholder until AliveCheck notification is specified.
		std::shared_ptr<UriNotif> notif = std::make_shared<UriNotif>(user_id, m_instance_id, now_timestamp, notification, "");
		// Send notifications.
		notif->sendGcm(&sr_ids);
		logger.LOGFILE("alive_check", "INFO") << "Instance AliveCheck: " << m_instance_id << " sent notifification: [user_id: " << user_id
				  << ", timestamp: " << now_timestamp << ", notification: " << notification << "]" << std::endl;
	}
}
