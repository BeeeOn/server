#include "locked_stream.h"


 /**
 * @brief Gets time used in logs
 * @return string representation of current date
 * Format: [YYYY-MM-DD HH:MM:SS:mmm]
 */
std::string getTime(void)
{
    timeval now;
    gettimeofday(&now, 0);
    tm *curr_time = localtime(&now.tv_sec);
    int ms = now.tv_usec/1000;
    std::ostringstream buff;

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
    buff << curr_time->tm_sec << ":";

    if(ms<100){buff << "0";}
    buff << ms << ")";
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


locked_stream::locked_stream(std::ostream& stream, std::string tag, std::string severity, std::string location, int line)
    : lock_(s_out_mutex),
    stream_(&stream)
{
    setup(stream, tag, severity, location, line);
}


locked_stream::locked_stream(locked_stream&& other)
    : lock_(std::move(other.lock_)),
    stream_(other.stream_)
{
    other.stream_ = nullptr;
}

/* End of locked_stream.cpp */