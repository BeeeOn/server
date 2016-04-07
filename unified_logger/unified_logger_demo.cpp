#include "unified_logger.h"

Unified_logger logger("framework"); //default log folder ".", default log level ERROR
std::mutex locked_stream::s_out_mutex{};

void behavior1()
{
    logger.out(__FILE__, __LINE__, "humidity", "INFO") << "Parameters: unit id: " << 124 << " status: " << 42 << " message: " << "accepted" << std::endl;
    logger.LOGOUT("temperature", "DEBUG") << "Entered config state" << std::endl; 
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

