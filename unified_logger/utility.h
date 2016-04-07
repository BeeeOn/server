#ifndef UNIFIED_LOGGER_UTILITY_H
#define UNIFIED_LOGGER_UTILITY_H    

/**
 * utility.h
 * utility class for unified logger library
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */ 

#include <string>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>

    //get actual time
    //Format: [7.3.2016 21:54:59:382]
    std::string getTime(void);

    //gets filename from date
    //Format: [tag]yymmdd.log
    std::string getFileNameByDate();

#endif /* UNIFIED_LOGGER_UTILITY_H */ 