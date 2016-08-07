/*
 * File:   TimedTaskInstance.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 22. January 2016
 */

#ifndef TIMEDTASKINSTANCE_H
#define TIMEDTASKINSTANCE_H

#include <chrono>
#include <memory>
#include <mutex>
#include <set>
#include <string>

#include "TaskInstance.h"

class TimedTaskInstance: public virtual TaskInstance
{
public:
	/**
	 * Constructor of class TimedTaskInstance.
	 * @param instance_id Identificator of created instance.
	 * @param owning_manager Pointer to manager of this instance.
	 */
	TimedTaskInstance(long instance_id, std::weak_ptr<TaskManager> owning_manager);
	/**
	 * Destructor of class TimedTaskInstance.
	 */
	virtual ~TimedTaskInstance();
	/**
	 * Plans to Calendar after how many seconds should instance be activated.
	 * @param seconds Seconds after which activate instance.
	 */
	void planActivationAfterSeconds(int seconds);
	/*
	 * Plans to Calendar activation of instance to present time.
	 */
	void planActivationNow();
	/**
	 * Plans instance activation to Calendar to exact date and time.
	 * It must have format: "1 9 2014 12:35:34" -> "month day_of_month year time"
	 */
	void planActivationToDateAndTime(std::string date_time);
	/**
	 * Activates an instance (calls run() function, but protects it with binary semaphore).
	 * @param activation_time Time at which instance was activated.
	 */
	void activate(std::chrono::system_clock::time_point activation_time) override;
	/**
	 * Removes all planned activations of instance from Calendar.
	 */
	virtual void deleteFromControlComponent() override;
	/**
	 * Removes planned activation of instance from Calendar.
	 * @param activation_time Time of planned activation.
	 */
	void removePlannedActivation(std::chrono::system_clock::time_point activation_time);
	/**
	 * This function should be implemented by specific task instance and should
	 * contain functionality defined by said task.
	 */
	virtual void run(std::chrono::system_clock::time_point activation_time) = 0;

protected:
	/**
	 * All times at which was instance planned to Calendar.
	 */
	std::set<std::chrono::system_clock::time_point> m_activation_times;
};

#endif /* TIMEDTASKMINSTANCE_H */
