#ifndef LOGGER_H
#define LOGGER_H
#include <pthread.h>
#include <string>
#include <cstring>
inline const char* getCurrentThreadName() {
    thread_local char name[16] = {};
    pthread_getname_np(pthread_self(), name, sizeof(name));
    return name;
}

inline void setCurrentThreadName(const std::string& name){
    pthread_setname_np(pthread_self(), name.substr(0, 15).c_str());
}

#define ENABLE_DEBUG_LOG
#ifdef ENABLE_DEBUG_LOG
#include <cstdio>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>

#define DEBUG_LOG(fmt, ...) \
    do { \
        std::fprintf(stderr, "[tid=%d][tname=%s][%s:%d][%s()] " fmt "\n", \
            static_cast<int>(gettid()), \
            getCurrentThreadName(), \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    } while (0)
#else
#define DEBUG_LOG(fmt, ...) do {} while (0)
#endif

#endif // LOGGER_H