#include "locked_stream.h"
/*
locked_stream::locked_stream(std::ostream& stream)
        : lock_(s_out_mutex)
        , stream_(&stream)
    { }

locked_stream& locked_stream::operator<<(int num)
{
    static std::mutex _out_mutex;
    std::unique_lock<std::mutex> lock(_out_mutex);
    //std::lock_guard<std::mutex> lock (_out_mutex);
    std::cout << std::to_string(num);
    return *this;
}

    locked_stream::locked_stream(locked_stream&& other)
        : lock_(std::move(other.lock_))
        , stream_(other.stream_)
    {
        other.stream_ = nullptr;
    }

    locked_stream&& locked_stream::operator << (locked_stream&& s, std::ostream& (*arg)(std::ostream&))
    {
        (*s.stream_) << arg;
        return std::move(s);
    }

    template <typename Arg>
    locked_stream&& locked_stream::operator << (locked_stream&& s, Arg&& arg)
    {
        (*s.stream_) << std::forward<Arg>(arg);
        return std::move(s);
    }*/