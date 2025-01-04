#pragma once
#include <chrono>
#include <iostream>
#include <string>

class Timer
{
public:
    Timer(const std::string &name = "") : name_(name)
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!name_.empty() && !printed_)
        {
            stop();
            std::cout << name_ << "耗时: " << getElapsedMilliseconds() << "ms" << std::endl;
            printed_ = true;
        }
    }

    void stop()
    {
        if (!is_stopped_)
        {
            end_ = std::chrono::high_resolution_clock::now();
            is_stopped_ = true;
        }
    }

    void start()
    {
        if (is_stopped_)
        {
            auto now = std::chrono::high_resolution_clock::now();
            total_duration_ += std::chrono::duration<double, std::milli>(end_ - start_).count();
            start_ = now;
            end_ = {};
            is_stopped_ = false;
        }
    }

    void reset()
    {
        start_ = std::chrono::high_resolution_clock::now();
        end_ = {};
        total_duration_ = 0.0;
        is_stopped_ = false;
        printed_ = false;
    }

    double getElapsedMilliseconds()
    {
        if (!is_stopped_)
        {
            stop();
        }
        return total_duration_ +
               std::chrono::duration<double, std::milli>(end_ - start_).count();
    }

private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
    std::chrono::high_resolution_clock::time_point end_{};
    double total_duration_ = 0.0;
    bool is_stopped_ = false;
    bool printed_ = false;
};