/*
 * File:   Calendar.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include "Calendar.h"

#include <chrono>
#include <ctime>
#include <memory>
#include <thread>
#include <vector>

#include "TaskInstance.h"
#include "Logger.h"

// Definition of singleton instance.
std::shared_ptr<Calendar> Calendar::m_instance;

Calendar::Calendar():
	m_wakeup_time(std::chrono::system_clock::now()),
	m_should_run(true),
	m_running(false)
{
}

Calendar::~Calendar()
{
}

void Calendar::createInstance()
{
	if (!m_instance) {
		logger.LOGFILE("calendar", "INFO") << "Calendar created." << std::endl;
		m_instance = std::shared_ptr<Calendar>(new Calendar);
	}
}

std::shared_ptr<Calendar> Calendar::getInstance()
{
	if (m_instance) {
		return m_instance;
	}
	else {
		throw std::runtime_error("Calendar singleton was not created or already destructed.");
	}
}

void Calendar::runCalendar()
{
	if (m_running) {
		return;
	}
	else {
		m_running = true;
	}
	// Stores all event which should be executed.
	std::multimap<std::chrono::system_clock::time_point, TaskInstance*> to_activate;

	// Run for all eternity (until whole system shuts down).
	while(m_should_run) {

		// If Calendar is empty, wait in thread until new activation comes.
		waitUntilCalendarIsNotEmpty();

		if (m_should_run == false) {
			// When stopping calendar which was waiting on empty calendar,
			// this safely stops algorithm.
			break;
		}

		m_calendar_events_mx.lock();

		// Save current time.
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

		while (m_calendar_events.begin()->first <= now) {
			// Stores every event with activation time less or equal to now.
			to_activate.emplace(m_calendar_events.begin()->first, m_calendar_events.begin()->second);

			// Erases stored event from Calendar.
			m_calendar_events.erase(m_calendar_events.begin());

			// If the Calendar is empty, stop loop immediately.
			if (m_calendar_events.empty()) {
				break;
			}
		}

		if(!m_calendar_events.empty()) {
			// Set the activation time of event with lowest activation time as time to wake up this thread.
			m_wakeup_time = m_calendar_events.begin()->first;
		}
		else {
			// If there is no event in Calendar this thread will won't wait in this iteration,
			// but in next iteration waits at waitUntilCalendarIsNotEmpty().
			m_wakeup_time = std::chrono::system_clock::now();
		}
		try {
			// Launch thread to execute stored events.
			std::thread t_execute_events(&Calendar::activateInstances, this, to_activate);
			t_execute_events.detach();
		}
		catch (const std::exception& e) {
			for(auto activation : to_activate) {
				// If creating of thread fails, put activations back to calendar.
				m_calendar_events.emplace(activation.first, activation.second);
			}
			logger.LOGFILE("calendar", "ERROR") << e.what() << std::endl;
		}

		to_activate.clear();

		// Creates lock by which can thread wake up if to queue comes event which has
		// lower activation time then the one for which thread waits now.
		std::unique_lock<std::mutex> lock(m_new_wakeup_time_mx);

		m_calendar_events_mx.unlock();
		// Wait until the activation time of event with a lowest activation time in queue.
		m_new_wakeup_time_cv.wait_until(lock, m_wakeup_time);
	}

	logger.LOGFILE("calendar", "INFO") << "Calendar algorithm stopped." << std::endl;
}

void Calendar::activateInstances(std::multimap<std::chrono::system_clock::time_point, TaskInstance*> to_activate)
{
	if(!to_activate.empty()) {
		for (auto instance : to_activate) {

			instance.second->activate(instance.first);
		}
	}
}

void Calendar::waitUntilCalendarIsNotEmpty()
{
	bool calendar_empty = m_calendar_events.empty();
	if (calendar_empty) {

		// Until it's not notified that there is an event in calendar, wait for event to be emplaced.
		std::unique_lock<std::mutex> lock(m_calendar_not_empty_mx);
		m_calendar_not_empty_cv.wait(lock);
	}
}

void Calendar::emplaceActivation(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr)
{
	m_calendar_events_mx.lock();
	// Before new event is created, check if Calendar is empty.
	bool calendar_empty = m_calendar_events.empty();

	// Create new event and push to Calendar.
	m_calendar_events.emplace(activation_time, instance_ptr);

	if (!calendar_empty) {

		// If activation time of pushed event is lower than time for which main Calendar
		// algorithm (run()) waits, it should wake up said thread so it can calculate new time to wake up.
		if (activation_time < m_wakeup_time ) {
			m_new_wakeup_time_cv.notify_all();
		}
	}
	else {
		// If Calendar is empty that means that main calendar algorithm (run())is waiting
		// in waitUntilCalendarIsNotEmpty() function and should be notified to wake up.
		m_calendar_not_empty_cv.notify_all();
	}
	m_calendar_events_mx.unlock();
}

std::chrono::system_clock::time_point Calendar::planActivation(int seconds, TaskInstance* instance_ptr)
{
	std::chrono::system_clock::time_point activation_time = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
	// Compute exact time when event should activate and push event.
	emplaceActivation(activation_time, instance_ptr);
	return activation_time;
}

std::chrono::system_clock::time_point Calendar::planActivation(TaskInstance* instance_ptr)
{
	// Push event at current time.
	std::chrono::system_clock::time_point activation_time = std::chrono::system_clock::now();
	emplaceActivation(activation_time, instance_ptr);
	return activation_time;
}

std::chrono::system_clock::time_point Calendar::planActivation(std::string date_time, TaskInstance* instance_ptr)
{
	std::tm tm = {};
	// Format: "1 9 2014 12:35:34" -> "month day_of_month year time"
	strptime(date_time.c_str(), "%m %d %Y %H:%M:%S", &tm);
	std::chrono::system_clock::time_point activation_time = std::chrono::system_clock::from_time_t(std::mktime(&tm));

	emplaceActivation(activation_time, instance_ptr);
	return activation_time;
}

void Calendar::stopCalendar()
{
	m_should_run = false;
	// Wakeup calendar algorithm to end it.
	m_new_wakeup_time_cv.notify_all();
	m_calendar_not_empty_cv.notify_all();
}

void Calendar::removeAllActivationsOfInstance(std::set<std::chrono::system_clock::time_point> planned_times, TaskInstance* instance_ptr)
{
	for (auto activation_time : planned_times) {
		removeActivation(activation_time, instance_ptr);
	}
}

void Calendar::removeActivation(std::chrono::system_clock::time_point activation_time, TaskInstance* instance_ptr)
{
	std::lock_guard<std::mutex> lock(m_calendar_events_mx);

	// Find range of all events containing activation_time as key.
	auto range_of_keys =  m_calendar_events.equal_range(activation_time);
	// Iterate over all events and remove those by instance_ptr.
	for (auto it = range_of_keys.first; it != range_of_keys.second; it++) {

		if (it->second == instance_ptr) {
			m_calendar_events.erase(it);
		}
	}
}
