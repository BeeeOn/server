/* 
 * File:   Calendar.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 * 
 * Created on 20. January 2016
 */

#include "Calendar.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include "CalendarEvent.h"

// Definition of static variables.
std::mutex Calendar::m_calendar_events_mx, Calendar::m_new_wakeup_time_mx, Calendar::m_queue_not_empty_mx, Calendar::m_test_queue_empty_mx;
std::condition_variable Calendar::m_new_wakeup_time_cv, Calendar::m_queue_not_empty_cv;
std::chrono::system_clock::time_point Calendar::m_wakeup_time = std::chrono::system_clock::now();
std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> Calendar::m_calendar_events;
bool Calendar::m_should_run(true);

Calendar::Calendar()
{
}

Calendar::~Calendar() {
}

void Calendar::run() {
    // Stores all event which should be executed.
    std::vector<std::shared_ptr<CalendarEvent>> events_to_execute;
    
    // Run for all eternity (until whole system shuts down).  
    while(m_should_run) {
        
        // If event queue is empty, wait in thread until event comes.
        waitUntilCalendarIsNotEmpty();
        
        m_calendar_events_mx.lock(); 
        
        // Save current time.
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        
        while(m_calendar_events.top()->getActivationTime() <= now) {
            
            // Stores every event with activation time less or equal to now.
            events_to_execute.push_back(m_calendar_events.top());

            // Pops stored event from queue.
            m_calendar_events.pop();
            
            // If the queue is empty, stop loop immediately.
            if(m_calendar_events.empty())
                break;
        }
        
        if(!m_calendar_events.empty()) {
            // Set the activation time of event with lowest activation time as time to wake up this thread.
            m_wakeup_time = m_calendar_events.top()->getActivationTime();
        }
        else {
            // If there is no event in queue this thread will won't wait in this iteration,
            // but in next iteration waits at waitUntilCalendarIsNotEmpty().
            m_wakeup_time = std::chrono::system_clock::now();
        }
        
        m_calendar_events_mx.unlock();
        
        // Launch thread to execute stored events.
        std::thread t_execute_events(&Calendar::executeEvents, this, events_to_execute);
        t_execute_events.detach();

        events_to_execute.clear();
        
        // Creates lock by which can thread wake up if to queue comes event which has
        // lower activation time then the one for which thread waits now.
        std::unique_lock<std::mutex> lock(m_new_wakeup_time_mx);
        
        // Wait until the activation time of event with a lowest activation time in queue.
        m_new_wakeup_time_cv.wait_until(lock, m_wakeup_time);
    }
    
    std::cout << "CALENDAR ALGORITHM STOPPED!!." << std::endl;
    // HERE CAN BE STORED EVERYTHING IN CALENDAR.
}

void Calendar::executeEvents(std::vector<std::shared_ptr<CalendarEvent>> events_to_execute) {
    if(!events_to_execute.empty()) {
        for (auto event : events_to_execute) {
            // Print time of activation and activate instances.
            time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "Instance activated at time: " << ctime(&tn) << std::endl;
            event->activateInstance();
        }
    }
}

void Calendar::waitUntilCalendarIsNotEmpty() {
    m_test_queue_empty_mx.lock();
    
    bool calendar_empty = m_calendar_events.empty();
    if (calendar_empty) {
        
        std::cout << "Calendar empty, wait for event. " << m_calendar_events.size() << "->" << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
        
        // Until it's not notified that there is an event in calendar, wait for event to be emplaced.
        std::unique_lock<std::mutex> lock(m_queue_not_empty_mx);
        m_test_queue_empty_mx.unlock();
        m_queue_not_empty_cv.wait(lock);
    }
    if (!calendar_empty) {
        m_test_queue_empty_mx.unlock();
        std::cout << "Calendar not empty, run." << m_calendar_events.size() << " ->" << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
    }
    
}

void Calendar::pushEvent(std::chrono::system_clock::time_point activation_time, TimedTaskInstance* instance_ptr) {
    
    m_calendar_events_mx.lock();
    m_test_queue_empty_mx.lock();
    // Before new event is created, check if queue is empty.
    bool calendar_empty = m_calendar_events.empty();
    
    // Create new event and push to queue.
    m_calendar_events.push(std::make_shared<CalendarEvent>(activation_time, instance_ptr));
    
    std::cout << "PUSH ->" << std::chrono::system_clock::now().time_since_epoch().count() << std::endl;
    
    if (!calendar_empty) {
       
        // If activation time of pushed event is lower than time for which main calendar algorithm (run()) waits,
        // it should wake up said thread so it can calculate new time to wake up.
        if (activation_time < m_wakeup_time ) {
            m_new_wakeup_time_cv.notify_all();
        }
    }
    else {
        // If queue is empty that means that main calendar algorithm (run())is waiting
        // in waitUntilCalendarIsNotEmpty() function and should be notified to wake up.
        m_queue_not_empty_cv.notify_all();
    }
    m_test_queue_empty_mx.unlock();
    m_calendar_events_mx.unlock();
}

void Calendar::emplaceEvent(int seconds, TimedTaskInstance *instance_ptr) {
    // Compute exact time when event should activate and push event.
    pushEvent(std::chrono::system_clock::now() + std::chrono::seconds(seconds), instance_ptr);
}

void Calendar::emplaceEvent(TimedTaskInstance* instance_ptr) {
    // Push event at current time.
    pushEvent(std::chrono::system_clock::now(), instance_ptr);
}

void Calendar::stopCalendar() {
    m_should_run = false;
    // Wakeup calendar algorithm to end.
    m_new_wakeup_time_cv.notify_all();
}
