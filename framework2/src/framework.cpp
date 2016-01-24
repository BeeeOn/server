/* 
 * File:   framework.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include <chrono>
#include <thread>

//#include <asio.hpp>

#include "Calendar.h"
#include "Logger.h"
#include "ManagerLoader.h"
#include "Server.h"

#include "TimedAlgorithmManager.h"

int main(int argc, char** argv) {

    std::string clientDelim("</adapter_server>"); // konec xml od adapteru
    std::string serverDelim("</end>");
    int threadCount = 10;
    int port = 8989;
    int timePeriod = 5;
    
    Logger logger();
  
    ManagerLoader manager_loader;
    // In a future pass path to algorithm config file.
    manager_loader.loadAlgorithmManagers();
    

    Calendar calendar;

    std::thread t_calendar(&Calendar::run, &calendar);
    t_calendar.detach();
    
    time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "START: " << ctime(&tn) << std::endl;
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    /*
    Calendar::emplaceEvent(3, "Event 1.");
    Calendar::emplaceEvent(3, "Event 2.");
    
    //std::this_thread::sleep_for(std::chrono::seconds(7));
    
    Calendar::emplaceEvent(3, "Event 3.");
    Calendar::emplaceEvent(3, "Event 4.");
    
    //std::this_thread::sleep_for(std::chrono::seconds(15));
    Calendar::emplaceEvent(3, "Event 5.");
    //calendar.startTimedManagers(&);
   
    std::shared_ptr<TimedAlgorithmInstance> instance;
    std::cout << "PTR:::::" << instance.get() << std::endl;
    std::cout << "THIS::::" << instance->getptr().get() << std::endl;
    */
    // Server part.
    asio::io_service io_service;
    
    Server s(io_service, port,
      clientDelim, serverDelim,
      threadCount, timePeriod);
    
    s.startAccept();
    s.run();
    
    //t_calendar.join();
    
    std::this_thread::sleep_for(std::chrono::seconds(100));
    return 0;
}