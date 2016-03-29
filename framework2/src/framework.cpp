/* 
 * File:   framework.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include <asio.hpp>
#include <boost/bind.hpp>

#include <soci.h>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

#include "Calendar.h"
#include "ConfigParser.h"
#include "DatabaseInterface.h"
#include "DataMessageRegister.h"
#include "GatewayServer.h"
#include "TaskLoader.h"
#include "UserServer.h"

void stopBAF(const asio::error_code& error, UserServer *user_server, GatewayServer *gateway_server)
{
  if (!error)
  {
    Calendar::stopCalendar();
    user_server->handleStop();
    gateway_server->handleStop();
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
    
    std::cout << "Create database interface." << std::endl;
    std::shared_ptr<DatabaseInterface> database_interface = DatabaseInterface::getInstance();
    try {
        std::cout << "Connecting to database." << std::endl;
        DatabaseInterface::getInstance()->connectToDatabase(config_parser.m_database_sessions, config_parser.m_database_connection_string);
        std::cout << "Connected to database." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Probably couldn't connect to database." << std::endl;
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    std::string given_name;
    std::shared_ptr<soci::session> sql = DatabaseInterface::getInstance()->makeNewSession();
    *sql << "SELECT given_name FROM users WHERE user_id=6", soci::into(given_name);
    std::cout << "ZISKANE JMENO: " << given_name << std::endl;
    
    // Starts calendar algorithm.
    Calendar calendar;
    std::thread calendar_thread(&Calendar::run, &calendar);
    calendar_thread.detach();
    
    // Create DataMessageRegister.
    std::shared_ptr<DataMessageRegister> data_message_register = DataMessageRegister::getInstance();   
    
    // Loads algorithm managers.
    std::shared_ptr<TaskLoader> task_loader = TaskLoader::getInstance();
    try {
        // In a future pass path to algorithm config file.
        //manager_loader.loadAlgorithmManagers();
        TaskLoader::getInstance()->createAllTasks(config_parser.m_tasks_config_path);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    std::cout << "Number of tasks: " << TaskLoader::getInstance()->m_tasks.size() << std::endl;
    for (auto task : TaskLoader::getInstance()->m_tasks) {
        std::cout << "Name: " << task.second->getTaskName() << std::endl;
        
        //std::cout << "Creating new instance!" << std::endl;
        
        //task.second->getTaskManagerPtr()->createInstance(1, 1);
    }
    
    //std::string clientDelim("</adapter_server>"); // Delimeter of XML from gateway.
    //std::string serverDelim("</end>");
   
    // Initializes and starts server.
    asio::io_service io_service;
   
    std::cout << "START SERVERS" << std::endl;
    
    //Server gateway_server(io_service, config_parser.m_gateway_server_port, clientDelim, serverDelim, config_parser.m_gateway_server_threads, 5);
    //gateway_server.startAccept();
    //std::thread gateway_server_thread(&Server::run, &gateway_server);
    
    GatewayServer gateway_server(io_service, config_parser.m_gateway_server_port, config_parser.m_gateway_server_threads);
    gateway_server.startAccept();
    std::thread gateway_server_thread(&Server::run, &gateway_server);
    
    UserServer user_server(io_service, config_parser.m_user_server_port, config_parser.m_user_server_threads);
    user_server.startAccept();
    
    asio::signal_set signals(io_service);
    signals.add(SIGINT);
    signals.add(SIGTERM);
      #if defined(SIGQUIT)
    signals.add(SIGQUIT);
      #endif // defined(SIGQUIT)
    signals.async_wait(boost::bind(stopBAF, asio::placeholders::error, &user_server, &gateway_server));
    
    user_server.run();
    
    //calendar_thread.join();
    gateway_server_thread.join();
    
    std::cout << "SERVER STOPED!" << std::endl;
    return 0;
}