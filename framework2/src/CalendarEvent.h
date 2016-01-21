/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CalendarEvent.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 17:00
 */

#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H




class CalendarEvent {
public:
    CalendarEvent();
    CalendarEvent(const CalendarEvent& orig);
    virtual ~CalendarEvent();
    
    int activation_time;
    

    
private:
    
    
};
      /*
    bool operator<(const CalendarEvent& lhs, const CalendarEvent& rhs)
    {
      return lhs.activation_time < rhs.activation_time;
    }  
       */
#endif /* CALENDAREVENT_H */

