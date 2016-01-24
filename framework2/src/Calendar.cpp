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
#include "Logger.h"
#include "ManagerLoader.h"

// Definition of static variables.
std::mutex Calendar::m_calendar_events_mutex;
std::chrono::system_clock::time_point Calendar::m_wakeup_time = std::chrono::system_clock::now();
std::priority_queue<std::shared_ptr<CalendarEvent>, std::vector<std::shared_ptr<CalendarEvent>>, GreaterCalendarEventSharedPtr> Calendar::m_calendar_events;
std::condition_variable Calendar::cv, Calendar::empty_queue_cv;
std::mutex Calendar::cv_m , Calendar::empty_queue_m;

Calendar::Calendar()
{
}

Calendar::Calendar(const Calendar& orig) {
}

Calendar::~Calendar() {
}

void Calendar::run() {
    
    std::vector<std::shared_ptr<CalendarEvent>> events_to_execute;
    
    // Run for all eternity (until whole system shuts down).  
    while(true) {
        
        // If event queue is empty, wait in thread until event comes.
        waitUntilCalendarIsNotEmpty();
        
        // Lock queue.
        m_calendar_events_mutex.lock(); 
        
        // Store current time.
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        
        
        while(m_calendar_events.top()->getActivationTime() <= now) {
            // ziskani eventu s nizsim nebo stejnym casem jako now

            //events_to_execute.push_back();
            /*std::shared_ptr<CalendarEvent> ev_ptr = m_calendar_events.top();
            CalendarEvent event;
            event = *ev_ptr;
            */
            
            //std::shared_ptr<Event> e = std::make_shared<Event>();
            //std::shared_ptr<CalendarEvent> o = std::make_shared);
            
            events_to_execute.push_back(std::make_shared<CalendarEvent>(*(m_calendar_events.top())));
            
            //std::cout << "ACTION: MANAGER:" << events_to_execute.back()->m_manager_id << " INSTANCE: " << events_to_execute.back()->m_manager_id << std::endl;
            //time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            //std::cout << "ACTION TIME: " << ctime(&tn) << std::endl;
    
            //std::cout << "ACTION:" << m_calendar_events.top()->getText() << std::endl;
               
            //std::cout << "ACTION:" << events_to_execute.back().getText() << std::endl;
            // Pop stored event.
            m_calendar_events.pop();
            
            //std::cout << "ACTION:" << o->getText() << std::endl;
            
            // If queue is empty, stop loop immediately.
            if(m_calendar_events.empty())
                break;
        }
        
        if(!m_calendar_events.empty()) {
            m_wakeup_time = m_calendar_events.top()->getActivationTime();
        }
        else {
            m_wakeup_time = std::chrono::system_clock::now();
        }
        
        m_calendar_events_mutex.unlock();
        
        std::thread t_execute_events(&Calendar::executeEvents, this, events_to_execute);
        t_execute_events.detach();

        events_to_execute.clear();
        
        // If sleep time is not greater than zero, execute events immediately.
        //if (sleep_time_ms > 0)
        //    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_ms));
        //std::this_thread::sleep_until(wakeup_time);
        
        
        std::unique_lock<std::mutex> lk(cv_m);
        
        cv.wait_until(lk, m_wakeup_time);
        //std::cout << "Calendar finished waiting." << std::endl;  
    }
}

void Calendar::executeEvents(std::vector<std::shared_ptr<CalendarEvent>> events_to_execute) {
    if(!events_to_execute.empty()) {
        for (auto event : events_to_execute) {
            
            time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << "ACTION TIME: " << ctime(&tn) << std::endl;
            ManagerLoader::activateInstance(event->m_manager_id, event->m_instance_id);
            //std::cout << "ACTIVATED" << std
        }
    }
}

void Calendar::waitUntilCalendarIsNotEmpty() {
    // Until there is no event in calendar, check every second for event.
    if (m_calendar_events.empty()) {
        std::cout << "Calendar empty, wait for event." << std::endl;
        std::unique_lock<std::mutex> lk(empty_queue_m);
        empty_queue_cv.wait(lk);
    }
    std::cout << "Calendar not empty, run." << std::endl;
    /*
    while(true) {
        if (m_calendar_events.empty()) {
            std::cout << "No event -> sleep a second." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        else {
            std::cout << "Event in queue -> wake up." << std::endl;
            break;
        }
    } 
    */ 
}
/*
long Calendar::calculateSleepTimeMs() {
    // Returns time for which should event picker sleep.
    if(!m_calendar_events.empty()) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_calendar_events.top()->getActivationTime().time_since_epoch()).count()
               - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    else {
        return 0; // There are no events in calendar, thread starts to wait.
    }
}
*/
/*
void Calendar::emplaceEvent(int wait_time, std::string text) {
    
    // Compute exact time when event should activate.
    std::chrono::system_clock::time_point activation_time = std::chrono::system_clock::now() + std::chrono::seconds(wait_time);
    
    m_calendar_events_mutex.lock();
    
    if (!m_calendar_events.empty()) {
        
        // If calendar thread sleeps and new event should be executed before m_wakeup_time, wake up thread.
        if (activation_time < m_wakeup_time ) {
            cv.notify_all();
        }
    }
    else {
        // Wake up calendar thread from waiting for event, there is event waiting. 
        empty_queue_cv.notify_all();
    }
    
    m_calendar_events.push(std::make_shared<CalendarEvent>(activation_time, text));
    
    m_calendar_events_mutex.unlock();
}
*/
void Calendar::emplaceEvent(int wait_time, unsigned int manager_id, unsigned long instance_id) {
    
    // Compute exact time when event should activate.
    std::chrono::system_clock::time_point activation_time = std::chrono::system_clock::now() + std::chrono::seconds(wait_time);
    
    m_calendar_events_mutex.lock();
    
    if (!m_calendar_events.empty()) {
        
        // If calendar thread sleeps and new event should be executed before m_wakeup_time, wake up thread.
        if (activation_time < m_wakeup_time ) {
            cv.notify_all();
        }
    }
    else {
        // Wake up calendar thread from waiting for event, there is event waiting. 
        empty_queue_cv.notify_all();
    }
    
    m_calendar_events.push(std::make_shared<CalendarEvent>(activation_time, manager_id, instance_id));
    
    m_calendar_events_mutex.unlock();
}