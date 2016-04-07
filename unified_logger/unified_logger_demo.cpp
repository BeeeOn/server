#include "unified_logger.h"
/**
 * unified_logger_demo.cpp
 * Library used for logging in unified format in multi-thread applications
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */  


Unified_logger logger("framework"); //default log folder ".", default log level WARN
std::mutex locked_stream::s_out_mutex{};

void behavior1()
{
    logger.out(__FILE__, __LINE__, "manager", "TRACE") << __FUNCTION__   << std::endl;
    logger.LOGOUT("temperature", "DEBUG") << "Entered config state" << std::endl; 
    logger.LOGOUT("manager", "WARN") << "Non-standart task name: " << 10 << std::endl;
    logger.LOGOUT("temperature", "INFO") << "Parameters: unit id: " << 124 << " status: " << 42 << " message: " << "accepted" << std::endl;
    logger.LOGOUT("manager", "ERROR") << "Can't load humidity module" << std::endl;
    logger.LOGOUT("pressure", "FATAL") << "Incorrect config: ID not positive value: " << 0 << std::endl;
}

void behavior2()
{
    logger.file(__FILE__, __LINE__, "manager", "TRACE") << __FUNCTION__   << std::endl;
    logger.LOGFILE("manager", "INFO") << "Manager initialized" << std::endl;
}

int main(void)
{
    std::vector<std::thread> threads;
    threads.push_back(std::thread(behavior1));
    
    threads.push_back(std::thread(behavior2));
    for(auto& thread : threads){
        thread.join();
    }
    return 0;
}

