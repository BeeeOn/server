#include "unified_logger.h"
/**
 * unified_logger_demo.cpp
 * Library used for logging in unified format in multi-thread applications
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */  

//default log folder ".", default log level WARN
//Unified_logger logger("framework"); 

// See all messages
//Unified_logger logger("framework", ".", LogSeverity::ALL);

// See INFO and above, log to path
Unified_logger logger("framework", "/var/log/mydebug", LogSeverity::INFO);


std::mutex locked_stream::s_out_mutex{};


//Demonstrates logging to stdout
void behavior1()
{
    logger.out(__FILE__, __LINE__, "manager", "TRACE") << __FUNCTION__   << std::endl;
    logger.LOGOUT("temperature", "DEBUG") << "Entered config state" << std::endl; 
    logger.LOGOUT("temperature", "MSG") << "Received message: <msg>42</msg>" << std::endl;
    logger.LOGOUT("manager", "WARN") << "Non-standart task name: " << 10 << std::endl;
    logger.LOGOUT("temperature", "INFO") << "Parameters: unit id: " << 124 << " status: " << 42 << " message: " << "accepted" << std::endl;
    logger.LOGOUT("manager", "ERROR") << "Can't load humidity module" << std::endl;
    logger.LOGOUT("pressure", "FATAL") << "Incorrect config: ID not positive value: " << 0 << std::endl;
}

//Demonstrates logginf to file
void behavior2()
{
    logger.file(__FILE__, __LINE__, "manager", "TRACE") << __FUNCTION__   << std::endl;
    logger.LOGFILE("manager", "INFO") << "Manager initialized" << std::endl;
}

int main(void)
{
    // Minimum log level can be adjusted manually
    logger.setLogLevel(LogSeverity::DEBUG);

    // Log folder path can be adjusted manually
    logger.setLogFolderPath(".");

    std::vector<std::thread> threads;
    threads.push_back(std::thread(behavior1));    
    threads.push_back(std::thread(behavior2));
    for(auto& thread : threads){
        thread.join();
    }
    return 0;
}

/* End of unified_logger_demo.cpp */