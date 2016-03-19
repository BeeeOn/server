/* 
 * File:   framework.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>

#include <asio.hpp>
#include <boost/bind.hpp>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

#include "Calendar.h"
#include "ConfigParser.h"
#include "TaskLoader.h"
#include "UserServer.h"

void stopBAF(const asio::error_code& error, UserServer *user_server/*, Server *gateway_server*/)
{
  if (!error)
  {
    Calendar::stopCalendar();
    user_server->handleStop();
    //gateway_server->handleStop();
  }
}

int main(int argc, char** argv) {
    // Prints time at which was framework started.
    time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "Framework start time: " << ctime(&tn) << std::endl;
    
    if (argc == 1) {
        std::cerr << "Path to config file must be provided in the first parameter." << std::endl;
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    else if (argc != 2) {
        std::cerr << "Wrong number of parameters was given." << std::endl;
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    ConfigParser config_parser;
    try {
        config_parser.parseConfigFile(argv[1]);
        
        std::cout << std::endl;
        std::cout << "TASK CONFIG PATH: " << config_parser.m_tasks_config_path << std::endl;
        std::cout << "USER SERVER THREADS: " << config_parser.m_user_server_threads << std::endl;
        std::cout << "USER SERVER PORT: " << config_parser.m_user_server_port << std::endl;
        std::cout << "GATEWAY SERVER THREADS: " << config_parser.m_gateway_server_threads << std::endl;
        std::cout << "GATEWAY SERVER PORT: " << config_parser.m_gateway_server_port << std::endl;
        std::cout << std::endl;
        
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    // Starts calendar algorithm.
    Calendar calendar;
    std::thread calendar_thread(&Calendar::run, &calendar);
    //t_calendar.detach();
    
    // Loads algorithm managers.
    TaskLoader task_loader;
    try {
        // In a future pass path to algorithm config file.
        //manager_loader.loadAlgorithmManagers();
        task_loader.createAllTasks(config_parser.m_tasks_config_path);
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
    
    std::string clientDelim("</adapter_server>"); // Delimeter of XML from gateway.
    std::string serverDelim("</end>");
   
    // Initializes and starts server.
    asio::io_service io_service;
   
    std::cout << "START SERVERS" << std::endl;
    
    //Server gateway_server(io_service, config_parser.m_gateway_server_port, clientDelim, serverDelim, config_parser.m_gateway_server_threads, 5);
    //gateway_server.startAccept();
    //std::thread gateway_server_thread(&Server::run, &gateway_server);
    
    UserServer user_server(io_service, config_parser.m_user_server_port, config_parser.m_user_server_threads);
    user_server.startAccept();
    
    asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);
      #if defined(SIGQUIT)
    signals.add(SIGQUIT);
      #endif // defined(SIGQUIT)
    signals.async_wait(boost::bind(stopBAF, asio::placeholders::error, &user_server/*, &gateway_server*/));
    
    user_server.run();
    
    calendar_thread.join();
    //gateway_server_thread.join();
    
    return 0;
}