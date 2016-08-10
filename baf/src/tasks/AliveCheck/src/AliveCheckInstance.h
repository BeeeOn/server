/*
 * File:   AliveCheckInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 21. January 2016
 */

#ifndef ALIVECHECKINSTANCE_H
#define ALIVECHECKINSTANCE_H

#include <chrono>
#include <memory>

#include "../../../model/TimedTaskInstance.h"
#include "../../../core/TaskManager.h"

#include "AliveCheckManager.h"

class AliveCheckInstance: public TimedTaskInstance
{
public:
	/**
	 * Constructor of class AliveCheckInstance.
	 */
	AliveCheckInstance(long instance_id,
					   std::weak_ptr<TaskManager> owning_manager);
	/**
	 * Destructor of class AliveCheckInstance.
	 */
	virtual ~AliveCheckInstance();
	/**
	 * Entry point for task main algorithm.
	 * @param activation_time Time at which was instance activated.
	 */
	void run(std::chrono::system_clock::time_point activation_time) override;

private:
	/**
	 * Contains main logic of task.
	 */
	void runAliveCheck();
	/**
	 * Sends notification to all users who has access to gateway.
	 * @param notification Text of notification to send.
	 */
	void sendUnavailableNotification(std::string notification);
	/**
	 * Gets gateway_id from configration from database.
	 * @return ID of checked gateway.
	 */
	long long getGatewayId();
	/**
	 * Checks if gateway is online.
	 * @param gateway_id ID of checked gateway.
	 * @param send_notif Item from configuration if user wants to receive notifications.
	 */
	void checkGatewayStatus(long long gateway_id, short send_notif);
};
#endif /* ALIVECHECKINSTANCE_H */
