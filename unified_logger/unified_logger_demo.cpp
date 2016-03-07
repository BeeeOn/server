#include "unified_logger.h"

int main(void)
{

    //stdout logging
    Unified_logger out_logger("out_prototype");

    out_logger.log(1, "Test message 1");
    out_logger.log(2, "Test message 2");
    out_logger.log(3, "Test message 3");
    out_logger.log(4, "Test message 4");
    out_logger.log(5, "Test message 5");
    out_logger.log(6, "Test message 6");

    //infile logging
    Unified_logger in_logger(false, ".", "in_prototype", WARN);

    in_logger.log(1, "Test message 1");
    in_logger.log(2, "Test message 2");
    in_logger.log(3, "Test message 3");
    in_logger.log(4, "Test message 4");
    in_logger.log(5, "Test message 5");
    in_logger.log(6, "Test message 6");



    //std::ofstream logfile;
    //logfile.open("log_test.log", std::ios::app);
    //if (logfile.is_open())
    //{
        //logfile << getTime();
        //logfile << "info" << " " << "no_tag" << " " << "\"Test message\"" << std::endl;
        //fprintf(logfile, "%s %s %s %s\n", getTime, "info", "no_tag", "\"Test message\"");
        //logfile.close();
    //}
    //else
    //{
    //    std::cout << "Unable to open log file" << std::endl;
    //}
    return 0;
}