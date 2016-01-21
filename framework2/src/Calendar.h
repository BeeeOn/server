/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Calendar.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 16:13
 */

#ifndef CALENDAR_H
#define CALENDAR_H

#include <queue>
#include <vector>

#include "CalendarEvent.h"

class Calendar {
public:
    Calendar();
    Calendar(const Calendar& orig);
    virtual ~Calendar();
private:
    
    //std::priority_queue<CalendarEvent, std::vector<CalendarEvent>> calendar_events;
    
    
};

#endif /* CALENDAR_H */

