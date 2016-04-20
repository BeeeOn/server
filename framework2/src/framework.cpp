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

#include "pugixml.hpp"
#include "pugiconfig.hpp"

#include "Calendar.h"
#include "Config.h"
#include "ConfigParser.h"
#include "DatabaseInterface.h"
#include "DataMessageRegister.h"
#include "GatewayServer.h"
#include "Logger.h"
#include "TaskLoader.h"
#include "UserServer.h"

// Definition of the extern logger object from Logger.h
Unified_logger logger("baf");
std::mutex locked_stream::s_out_mutex{};

void stopBAF(const asio::error_code& error, UserServer *user_server, GatewayServer *gateway_server)
{
    if (!error) {
        logger.LOGFILE("core", "INFO") << "BAF received SIGTERM, SIGINT or SIGQUIT signal. Shutting down BAF." << std::endl;
        
        user_server->stop();
        Calendar::getInstance()->stopCalendar(); 
        gateway_server->stop();
    }
}

void reloadTasks(const asio::error_code& error, asio::signal_set *reload_signal) {
    
    if (!error) {  
        try {
            TaskLoader::getInstance()->reloadTasksConfigFileAndFindNewTasks();
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    reload_signal->async_wait(boost::bind(reloadTasks, asio::placeholders::error, reload_signal));
}

int main(int argc, char** argv)
{
    // Check arguments of program.
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
    // Parse config file.
    ConfigParser config_parser;
    try {
        config_parser.parseConfigFile(argv[1]);
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    std::cout << "CONFIG: " << std::endl;
    std::cout << "tasks_config_path: " << Config::m_tasks_config_path << std::endl;
    std::cout << "user_server_threads: " << Config::m_user_server_threads << std::endl;
    std::cout << "user_server_port: " << Config::m_user_server_port << std::endl;
    std::cout << "gateway_server_threads: " << Config::m_gateway_server_threads << std::endl;
    std::cout << "gateway_server_port: " << Config::m_gateway_server_port << std::endl;
    std::cout << "database_sessions: " << Config::m_database_sessions << std::endl;
    std::cout << "database_connection_string: " << Config::m_database_connection_string << std::endl;
    std::cout << "log_folder_path: " << Config::m_log_folder_path << std::endl;
    std::cout << "log_level: " << Config::m_log_level << std::endl;
    std::cout << "ada_server_sender_port: " << Config::m_ada_server_sender_port << std::endl;
    
    // Setup Logger.
    try {
        LogSeverity severity = logger.StringToLevel(Config::m_log_level);
        logger.setLogLevel(severity);

        logger.setLogFolderPath(Config::m_log_folder_path);

        logger.LOGFILE("main", "INFO") << "Logger was set up. Folder path: " << Config::m_log_folder_path
                                       << ", Log level: " << Config::m_log_level << std::endl; 
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    std::cout << "Create database interface." << std::endl;
    DatabaseInterface::createInstance();
    try {
        std::cout << "Connecting to database." << std::endl;
        DatabaseInterface::getInstance()->connectToDatabase(Config::m_database_sessions, Config::m_database_connection_string);
        std::cout << "Connected to database." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Probably couldn't connect to database." << std::endl;
        std::cerr << e.what();
        std::cerr << "Shutting down BAF." << std::endl;
        return 1;
    }
    
    // Starts calendar algorithm.
    Calendar::createInstance();
    std::thread calendar_thread(&Calendar::runCalendar, Calendar::getInstance());
    //calendar_thread.detach();
    
    // Create DataMessageRegister.
    DataMessageRegister::createInstance();   
    
    // Loads algorithm managers.
    TaskLoader::createInstance();
    try {
        // In a future pass path to algorithm config file.
        //manager_loader.loadAlgorithmManagers();
        TaskLoader::getInstance()->createAllTasks(Config::m_tasks_config_path);
    }
    catch (const std::exception& e) {
        logger.LOGFILE("core", "FATAL") <<  e.what() << ": Shutting down BAF." << std::endl;
        return 1;
    }
    
    // Initializes and starts server.
    asio::io_service io_service;
   
    std::cout << "START SERVERS" << std::endl;

    GatewayServer gateway_server(io_service, Config::m_gateway_server_port, Config::m_gateway_server_threads);
    gateway_server.startAccept();
    std::thread gateway_server_thread(&GatewayServer::run, &gateway_server);
    
    UserServer user_server(io_service, Config::m_user_server_port, Config::m_user_server_threads);
    user_server.startAccept();
    
    asio::signal_set term_signals(io_service);
    term_signals.add(SIGINT);
    term_signals.add(SIGTERM);
      #if defined(SIGQUIT)
    term_signals.add(SIGQUIT);
      #endif // defined(SIGQUIT)
    term_signals.async_wait(boost::bind(stopBAF, asio::placeholders::error, &user_server, &gateway_server));
    
    /*
    [](const asio::error_code& error, UserServer *user_server, GatewayServer *gateway_server)
    {
        stopBAF(error, user_server, gateway_server);
    });
    */
    
    //
    
    asio::signal_set reload_signal(io_service);
    reload_signal.add(SIGUSR1);
    reload_signal.async_wait(boost::bind(reloadTasks, asio::placeholders::error, &reload_signal));
    
    user_server.run();
    
    gateway_server_thread.join();
    calendar_thread.join();
    
    std::cout << "END" << std::endl;
    
    return 0;
}