/* 
 * File:   framework.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include <chrono>
#include <stdexcept>
#include <thread>

//#include <asio.hpp>

#include "Calendar.h"
#include "TaskLoader.h"
#include "Server.h"

int main(int argc, char** argv) {
    
    std::string clientDelim("</adapter_server>"); // Delimeter of XML from gateway.
    std::string serverDelim("</end>");
    int threadCount = 10;
    int port = 8989;
    int timePeriod = 5;
  
    // Prints time at which was framework started.
    time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Framework start time: " << ctime(&tn) << std::endl;   
    
    // Starts calendar algorithm.
    Calendar calendar;
    std::thread t_calendar(&Calendar::run, &calendar);
    t_calendar.detach();
    
    // Loads algorithm managers.
    TaskLoader task_loader;
    try {
        // In a future pass path to algorithm config file.
        //manager_loader.loadAlgorithmManagers();
        task_loader.createAllTasks("/home/mrmaidx/server/framework2/tasks_conf.xml");
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    std::cout << "Number of tasks: " << task_loader.m_tasks.size() << std::endl;
    for (auto task: task_loader.m_tasks) {
        std::cout << "Name: " << task.second->getTaskName() << std::endl;
        std::cout << "Creating new instance!" << std::endl;
        task.second->getTaskManagerPtr()->createInstance(1, 1);
    }
    
    /*
    // Test of weak pointer.
    std::weak_ptr<int> weak_test;

    std::shared_ptr<int> cislo(std::make_shared<int>(5));

    weak_test = cislo;

    if(auto ptr = weak_test.lock()) {
        std::cerr << "Print pointer." << std::endl;
        std::cout << *ptr << std::endl;
    }
    else { 
        std::cerr << "Pointer is not pointing to anything." << std::endl;
    }
    */
    // Initializes and starts server.
    asio::io_service io_service;
    
    Server s(io_service, port,
      clientDelim, serverDelim,
      threadCount, timePeriod);
    
    s.startAccept();
    s.run();
    
    return 0;
}