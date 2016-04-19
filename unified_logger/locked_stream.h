#ifndef LOCKED_STREAM_H
#define LOCKED_STREAM_H

/**
 * locked_stream.h
 * Header only class for handling locked stdout using operator << 
 * @author Marek Beňo, xbenom01 at stud.fit.vutbr.cz
 * 7. April 2016
 */    

#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <sys/time.h>

std::string getTime(void);
std::string getFileNameByDate();

class locked_stream
{
public:

    //Initial stream, prints event header
    locked_stream(std::ostream& stream, std::string tag, std::string severity, 
        std::string location, int line);

    //handles all other <<
    locked_stream(locked_stream&& other);


    void setup(std::ostream& stream, std::string tag, std::string severity, std::string location, int line)
    {
        stream << getTime() << " " << tag << " [" << std::this_thread::get_id() << "] " << location << ":" << line << " " << severity  << " " ;
    }

    friend locked_stream&& operator << (locked_stream&& s, std::ostream& (*arg)(std::ostream&))
    {
        (*s.stream_) << arg;
        return std::move(s);
    }

    template <typename Arg>
    friend locked_stream&& operator << (locked_stream&& s, Arg&& arg)
    {
        (*s.stream_) << std::forward<Arg>(arg);
        return std::move(s);
    }
private:
    static std::mutex s_out_mutex;

    std::unique_lock<std::mutex> lock_;
    std::ostream* stream_;
};

#endif /* LOCKED_STREAM_H */ 