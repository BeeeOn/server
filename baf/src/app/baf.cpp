/*
 * File:   baf.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>

#include <asio.hpp>
#include <boost/bind.hpp>

#include "pugixml.hpp"
#include "pugiconfig.hpp"

#include "core/Calendar.h"
#include "app/Config.h"
#include "app/ConfigParser.h"
#include "server/DatabaseInterface.h"
#include "core/DataMessageRegister.h"
#include "server/GatewayServer.h"
#include "Logger.h"
#include "core/TaskLoader.h"
#include "server/UserServer.h"

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

void reloadTasks(const asio::error_code& error, asio::signal_set *reload_signal)
{
	logger.LOGFILE("core", "INFO") << "BAF received SIGUSR1, try to load new tasks from task config file."
			<< std::endl;
	if (!error) {
		try {
			TaskLoader::getInstance()->createNewTasks();
		}
		catch (const std::exception& e) {
			 logger.LOGFILE("core", "INFO") << e.what() << std::endl;
		}
	}
	// Wait for another SIGUSR1 signal.
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
	std::cout << "tasks_config_path: " << Config::tasks_config_path << std::endl;
	std::cout << "user_server_threads: " << Config::user_server_threads << std::endl;
	std::cout << "user_server_port: " << Config::user_server_port << std::endl;
	std::cout << "gateway_server_threads: " << Config::gateway_server_threads << std::endl;
	std::cout << "gateway_server_port: " << Config::gateway_server_port << std::endl;
	std::cout << "database_sessions: " << Config::database_sessions << std::endl;
	std::cout << "database_connection_string: " << Config::database_connection_string << std::endl;
	std::cout << "log_folder_path: " << Config::log_folder_path << std::endl;
	std::cout << "log_level: " << Config::log_level << std::endl;
	std::cout << "ada_server_sender_port: " << Config::ada_server_sender_port << std::endl;

	// Setup Logger.
	try {
		LogSeverity severity = logger.StringToLevel(Config::log_level);
		logger.setLogLevel(severity);

		logger.setLogFolderPath(Config::log_folder_path);

		logger.LOGFILE("core", "INFO") << "Logger was set up. Folder path: " << Config::log_folder_path
									   << ", Log level: " << Config::log_level << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
		std::cerr << "Shutting down BAF." << std::endl;
		return 1;
	}

	// Until this point it was not possible to use logger, because it was
	// not setup with information from config file.

	DatabaseInterface::createInstance();
	try {
		DatabaseInterface::getInstance()->connectToDatabase(Config::database_sessions, Config::database_connection_string);

	}
	catch (const std::exception& e) {
		logger.LOGFILE("core", "FATAL") << e.what()
				<< " Shutting down BAF." << std::endl;
		return 1;
	}

	// Create calendar and start calendar algorithm.
	Calendar::createInstance();
	std::thread calendar_thread(&Calendar::runCalendar, Calendar::getInstance());

	// Create DataMessageRegister.
	DataMessageRegister::createInstance();

	// Load algorithm managers.
	TaskLoader::createInstance();
	try {
		// In a future pass path to algorithm config file.
		//manager_loader.loadAlgorithmManagers();
		TaskLoader::getInstance()->createAllTasks(Config::tasks_config_path);
	}
	catch (const std::exception& e) {
		logger.LOGFILE("core", "FATAL") <<  e.what() << ": Shutting down BAF." << std::endl;
		return 1;
	}
	// Initializes and starts server.
	asio::io_service io_service;

	logger.LOGFILE("core", "INFO") << "START SERVERS" << std::endl;

	// Create data message interface.
	GatewayServer gateway_server(io_service, Config::gateway_server_port, Config::gateway_server_threads);
	gateway_server.startAccept();
	// Start runnig gateway server.
	std::thread gateway_server_thread(&GatewayServer::run, &gateway_server);

	// Create user message interface.
	UserServer user_server(io_service, Config::user_server_port, Config::user_server_threads);
	user_server.startAccept();

	// Set signal_set to catch termination signals.
	asio::signal_set term_signals(io_service);
	term_signals.add(SIGINT);
	term_signals.add(SIGTERM);
	  #if defined(SIGQUIT)
	term_signals.add(SIGQUIT);
	  #endif // defined(SIGQUIT)
	term_signals.async_wait(boost::bind(stopBAF, asio::placeholders::error, &user_server, &gateway_server));

	// Set signal_set to catch sigusr1 to reload new tasks.
	asio::signal_set reload_signal(io_service);
	reload_signal.add(SIGUSR1);
	reload_signal.async_wait(boost::bind(reloadTasks, asio::placeholders::error, &reload_signal));

	// Start runnig user server.
	user_server.run();

	// Join all threads.
	gateway_server_thread.join();
	calendar_thread.join();

	// Close all tasks.
	try {
		std::shared_ptr<TaskLoader> tl = std::dynamic_pointer_cast<TaskLoader>(TaskLoader::getInstance());
		tl->closeAllTasks();
	}
	catch (const std::exception& e) {
		logger.LOGFILE("core", "INFO") << e.what() << std::endl;
		return 1;
	}
	logger.LOGFILE("core", "INFO") << "BAF was shut down." << std::endl;
	return 0;
}
