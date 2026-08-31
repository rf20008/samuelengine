#pragma once
#ifndef SAMUELENGINE_ENSURE_HPP
#define SAMUELENGINE_ENSURE_HPP
#include <string>
#include <stdexcept>
#include <iostream>

[[noreturn]] inline void fail(const std::string& msg) {
    std::cerr << "ASSERT FAILED: " << msg << std::endl;
    throw std::runtime_error(msg);
}

#ifdef NDEBUG
// Release: do nothing
#define ENSURE(cond, msg) do { (void)sizeof(cond); (void)sizeof(msg); } while(0)
#else
// Debug: check
#define ENSURE(cond, msg) \
    do { \
        if (!(cond)) { \
            fail(std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + (msg)); \
        } \
    } while(0)
#endif
#endif