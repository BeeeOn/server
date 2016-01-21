/* 
 * File:   framework.cpp
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. ledna 2016, 19:19
 */

#include <cstdlib>

#include "Calendar.h"
#include "Logger.h"
#include "ManagerLoader.h"
#include "Server.h"

#include <asio.hpp>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    std::string clientDelim("</adapter_server>"); // konec xml od adapteru
    std::string serverDelim("</end>");
    int threadCount = 10;
    int port = 8989;
    int timePeriod = 5;
    
    Logger logger();
  
    ManagerLoader manager_loader;
    // In a future pass path to algorithm config file.
    manager_loader.loadAlgorithms();
    
    Logger::logToCout("Hello.");
    
    Calendar calendar;
    
    // Server part.
    asio::io_service io_service;
    
    Server s(io_service, port,
      clientDelim, serverDelim,
      threadCount, timePeriod);
    
    s.startAccept();
    s.run();
    
    return 0;
}