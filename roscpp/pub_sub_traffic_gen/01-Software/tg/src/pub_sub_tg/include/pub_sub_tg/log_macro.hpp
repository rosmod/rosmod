#ifndef LOG_MACRO_HPP
#define LOG_MACRO_HPP

#include <stdio.h>
#define TG_LOG(x,...) printf(x, ##__VA_ARGS__)

#endif
