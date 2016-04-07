#include "utility.h"

/**
 * @brief Gets time used in logs
 * @return string representation of current date
 * Format: [7.3.2016 21:54:59:382]
 */
std::string getTime(void)
{
    timeval now;
    gettimeofday(&now, 0);
    tm *curr_time = localtime(&now.tv_sec);

    std::ostringstream buff;

    //buff << "(" << curr_time->tm_mday << "." << curr_time->tm_mon+1 << "." << (curr_time->tm_year+1900) << " ";
    buff << "(" << (curr_time->tm_year+1900) <<  "-";
    if(curr_time->tm_mon+1<10){buff << "0";}
    buff << curr_time->tm_mon+1 << "-";
    if(curr_time->tm_mday+1<10){buff << "0";}
    buff << curr_time->tm_mday << " ";
    if(curr_time->tm_hour<10){buff << "0";}
    buff << curr_time->tm_hour << ":";

    if(curr_time->tm_min<10){buff << "0";}
    buff << curr_time->tm_min << ":";

    if(curr_time->tm_sec<10){buff << "0";}
    buff << curr_time->tm_sec << ":" << now.tv_usec/1000 << ")";
    return buff.str();
}

/**
 * @brief Gets filename from current date
 * @details Log file filename encodes current date in format: yymmdd
 * @return date encoded in string according to format
 */
std::string getFileNameByDate()
{
    timeval now;
    gettimeofday(&now, 0);
    tm *curr_time = localtime(&now.tv_sec);

    std::ostringstream buff;
    buff << (curr_time->tm_year+1900);
    if ( curr_time->tm_mon+1 < 10) { buff << "0";}
    buff << curr_time->tm_mon+1;
    if ( curr_time->tm_mday < 10) { buff << "0";}
    buff << curr_time->tm_mday;
    return buff.str();
}