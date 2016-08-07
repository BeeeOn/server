/*
 * File:   FireHazardInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 1. May 2016
 */

#ifndef FIREHAZARDINSTANCE_H
#define FIREHAZARDINSTANCE_H

#include <chrono>
#include <string>
#include <memory>

#include "../../../src/CombinedTaskInstance.h"
#include "../../../src/TaskManager.h"
#include "../../../src/DataMessage.h"

struct ActuatorInfo {
	long long a_gateway_id;
	long a_device_euid;
	int a_module_id;
};

class FireHazardInstance: public CombinedTaskInstance
{
public:
	FireHazardInstance(long instance_id,
					   std::weak_ptr<TaskManager> owning_manager,
					   long device_euid);

	virtual ~FireHazardInstance();
	/**
	 * Run function activated by received data.
	 * @param data_message Received senzoric data.
	 */
	void run(DataMessage data_message) override;
	/**
	 * Run function activated by time (calendar).
	 * @param activation_time Time of activation.
	 */
	void run(std::chrono::system_clock::time_point activation_time) override;
	/*
	 * This function is called when user changes device_euid from
	 * which instace should receive senzory data.
	 * It unregisters old euid and registers new one to DataMessageRegister.
	 */
	void changeRegisteredDeviceEuid(long device_euid);

private:
	/**
	 * Gets value of wanted module from received seonzoric data message.
	 * @param module_id ID of wanted module.
	 * @param data_message Received data message.
	 * @return Value of module.
	 */
	double getModuleValue(int module_id, DataMessage data_message);
	/*
	 * Defines in which state sequence of switching actuator is.
	 */
	short m_blink_sequence;
	/**
	 * Defines if sequence is activated (actuator is beeing blinked with).
	 */
	bool m_sequence_running;
	/**
	 * Gets information about actuator from database.
	 * @return Object with information about actuator.
	 */
	ActuatorInfo getActuatorInfo();
};

#endif /* FIREHAZARDINSTANCE_H */
