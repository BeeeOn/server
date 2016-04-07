#ifndef LOCKED_STREAM_H
#define LOCKED_STREAM_H    

#include <mutex>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>

#include "utility.h"
/*
class locked_stream
{
    static std::mutex s_out_mutex;

    std::unique_lock<std::mutex> lock_;
    std::ostream* stream_; // can't make this reference so we can move

public:

    locked_stream(std::ostream& stream);

    locked_stream& operator<<(int num);

    locked_stream(locked_stream&& other);

    friend locked_stream&& operator << (locked_stream&& s, std::ostream& (*arg)(std::ostream&));

    template <typename Arg>
    friend locked_stream&& operator << (locked_stream&& s, Arg&& arg);
};*/

class locked_stream
{
    static std::mutex s_out_mutex;

    std::unique_lock<std::mutex> lock_;
    std::ostream* stream_; // can't make this reference so we can move

public:
    locked_stream(std::ostream& stream, std::string tag, std::string severity, std::string location, int line)
        : lock_(s_out_mutex)
        , stream_(&stream)
    {
        std::cout << getTime() << " " << tag << " [" << std::this_thread::get_id() << "] " << location << ":" << line << " " << severity  << " " ;
     }

    locked_stream(locked_stream&& other)
        : lock_(std::move(other.lock_))
        , stream_(other.stream_)
    {
        other.stream_ = nullptr;
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
};


#endif /* LOCKED_STREAM_H */ 