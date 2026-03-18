// RBLogger.h
#pragma once

#include <string>

// Android special case：Release mode still logs errors
#if defined(__ANDROID__) && defined(ANDROID_RELEASE_WITH_ERROR_LOG)
    #include <android/log.h>
    #define ANDROID_LOG_ERROR_ONLY(msg) __android_log_write(ANDROID_LOG_ERROR, "VulkanApp", msg)
#else
    #define ANDROID_LOG_ERROR_ONLY(msg) ((void)0)
#endif

#ifndef RB_ENABLE_LOGGING
    #if defined(NDEBUG)
        #define RB_ENABLE_LOGGING 0
    #else
        #define RB_ENABLE_LOGGING 1
    #endif
#endif

#if RB_ENABLE_LOGGING

#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>

class Logger {
public:
    enum class Level { RB_DEBUG, RB_INFO, RB_WARNING, RB_ERROR, RB_FATAL };

    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void setLogFile(const std::string& filename) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_file.is_open()) m_file.close();
        m_file.open(filename, std::ios::app);
    }

    void setMinLevel(Level level) { m_minLevel = level; }

    void log(Level level, const std::string& message, 
             const char* file = nullptr, int line = 0) {
        if (level < m_minLevel) return;
        
        std::lock_guard<std::mutex> lock(m_mutex);
        
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        
        char time_buffer[64];
        std::strftime(time_buffer, sizeof(time_buffer), 
                     "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));
        
        std::string output = "[" + std::string(time_buffer) + "]" +
                            "[" + levelToString(level) + "]";
        
        if (file) {
            // get filename from full path
            std::string filename(file);
            size_t pos = filename.find_last_of("/\\");
            if (pos != std::string::npos) {
                filename = filename.substr(pos + 1);
            }
            output += "[" + filename + ":" + std::to_string(line) + "]";
        }
        
        output += " " + message + "\n";
        
        // console output
        std::cout << output;
        
        // file output
        if (m_file.is_open()) {
            m_file << output;
            m_file.flush();
        }
        
        // Android specific: also output to logcat
        #if defined(__ANDROID__)
            androidLog(level, output.c_str());
        #endif
    }

private:
    Logger() : m_minLevel(Level::RB_DEBUG) {
        // default log file
        setLogFile("vulkan_app.log");
    }
    ~Logger() { if (m_file.is_open()) m_file.close(); }
    
    std::ofstream m_file;
    std::mutex m_mutex;
    Level m_minLevel;
    
    std::string levelToString(Level level) {
        switch(level) {
            case Level::RB_DEBUG:   return "DEBUG";
            case Level::RB_INFO:    return "INFO";
            case Level::RB_WARNING: return "WARN";
            case Level::RB_ERROR:   return "ERROR";
            case Level::RB_FATAL:   return "FATAL";
            default: return "UNKNOWN";
        }
    }
    
    #if defined(__ANDROID__)
    void androidLog(Level level, const char* msg) {
        int androidLevel;
        switch(level) {
            case Level::RB_DEBUG:   androidLevel = ANDROID_LOG_DEBUG; break;
            case Level::RB_INFO:    androidLevel = ANDROID_LOG_INFO; break;
            case Level::RB_WARNING: androidLevel = ANDROID_LOG_WARN; break;
            case Level::RB_ERROR:   androidLevel = ANDROID_LOG_ERROR; break;
            case Level::RB_FATAL:   androidLevel = ANDROID_LOG_FATAL; break;
            default: androidLevel = ANDROID_LOG_DEBUG;
        }
        __android_log_write(androidLevel, "VulkanApp", msg);
    }
    #endif
};

// logger macro
#define RBLOG_DEBUG(msg) Logger::instance().log(Logger::Level::RB_DEBUG, msg, __FILE__, __LINE__)
#define RBLOG_INFO(msg)  Logger::instance().log(Logger::Level::RB_INFO, msg)
#define RBLOG_WARN(msg)  Logger::instance().log(Logger::Level::RB_WARNING, msg, __FILE__, __LINE__)
#define RBLOG_ERROR(msg) Logger::instance().log(Logger::Level::RB_ERROR, msg, __FILE__, __LINE__)
#define RBLOG_FATAL(msg) Logger::instance().log(Logger::Level::RB_FATAL, msg, __FILE__, __LINE__)

#else // RB_ENABLE_LOGGING == 0 (Release mode)

// Release mode: all log macros are empty
#define RBLOG_DEBUG(msg) ((void)0)
#define RBLOG_INFO(msg)  ((void)0)
#define RBLOG_WARN(msg)  ((void)0)
#define RBLOG_ERROR(msg) ANDROID_LOG_ERROR_ONLY(msg)  // Android keep error logs
#define RBLOG_FATAL(msg) ANDROID_LOG_ERROR_ONLY(msg)

// empty Logger class（keep interface）
class Logger {
public:
    enum class Level { RB_DEBUG, RB_INFO, RB_WARNING, RB_ERROR, RB_FATAL };
    
    static Logger& instance() {
        static Logger logger;
        return logger;
    }
    
    void setLogFile(const std::string&) {}
    void setMinLevel(Level) {}
};

#endif