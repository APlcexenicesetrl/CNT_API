/**
 * @file cnt/logging.h
 * Copyright 2025, aplcexenicesetrl project
 * MIT License
 */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <map>

#ifdef ERROR
#define _CNT_LOGGING_SAVED_ERROR_DEFINE_ ERROR
#undef ERROR
#endif

namespace cnt
{
    enum class LogColor
    {
        DEFAULT = 0,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37
    };

    enum class LogLevel
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };

    class Logger
    {
    public:
        explicit Logger(const std::string& name = "root")
            : name_(name),
            level_(LogLevel::INFO),
            format_("[{timestamp}] - {name} - {level} - {message}"),
            useColor_(true),
            consoleOutputEnabled_(true),
            file_(nullptr)
        {
            setDefaultColors();
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        Logger(Logger&& other) noexcept
            : name_(std::move(other.name_)),
            level_(other.level_),
            file_(std::move(other.file_)),
            format_(std::move(other.format_)),
            useColor_(other.useColor_),
            consoleOutputEnabled_(other.consoleOutputEnabled_),
            levelColors_(std::move(other.levelColors_))
        {
        }

        Logger& operator=(Logger&& other) noexcept
        {
            if (this != &other)
            {
                name_ = std::move(other.name_);
                level_ = other.level_;
                file_ = std::move(other.file_);
                format_ = std::move(other.format_);
                useColor_ = other.useColor_;
                consoleOutputEnabled_ = other.consoleOutputEnabled_;
                levelColors_ = std::move(other.levelColors_);
            }
            return *this;
        }

        // Configuration methods
        Logger& setFormat(const std::string& format)
        {
            format_ = format;
            return *this;
        }

        Logger& enableColor(bool enable)
        {
            useColor_ = enable;
            return *this;
        }

        Logger& setLevelColor(LogLevel level, LogColor color)
        {
            levelColors_[level] = color;
            return *this;
        }

        Logger& setLevel(LogLevel level)
        {
            level_ = level;
            return *this;
        }

        Logger& setOutputFile(const std::string& filename)
        {
            file_.reset(new std::ofstream(filename, std::ios::app));
            return *this;
        }

        Logger& enableConsoleOutput(bool enable)
        {
            consoleOutputEnabled_ = enable;
            return *this;
        }

        // Getters
        const std::string& getName() const { return name_; }
        LogLevel getLevel() const { return level_; }
        const std::string& getFormat() const { return format_; }
        bool isColorEnabled() const { return useColor_; }
        bool isConsoleOutputEnabled() const { return consoleOutputEnabled_; }
        const std::map<LogLevel, LogColor>& getLevelColors() const { return levelColors_; }
        std::ofstream& getOutputFile()
        {
            if (!file_) file_.reset(new std::ofstream);
            return *file_;
        }

    private:
        void setDefaultColors()
        {
            levelColors_ = {
                {LogLevel::DEBUG,    LogColor::CYAN},
                {LogLevel::INFO,     LogColor::GREEN},
                {LogLevel::WARNING,  LogColor::YELLOW},
                {LogLevel::ERROR,    LogColor::RED},
                {LogLevel::CRITICAL, LogColor::MAGENTA}
            };
        }

        std::string name_;
        LogLevel level_;
        std::unique_ptr<std::ofstream> file_;
        std::string format_;
        bool useColor_;
        bool consoleOutputEnabled_;
        std::map<LogLevel, LogColor> levelColors_;
    };

    class Logging
    {
    public:
        explicit Logging(Logger* logger) : logger_(*logger) {}

        template <typename... Args>
        void debug(const std::string& format, Args... args)
        {
            log(LogLevel::DEBUG, format, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(const std::string& format, Args... args)
        {
            log(LogLevel::INFO, format, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warning(const std::string& format, Args... args)
        {
            log(LogLevel::WARNING, format, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(const std::string& format, Args... args)
        {
            log(LogLevel::ERROR, format, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void critical(const std::string& format, Args... args)
        {
            log(LogLevel::CRITICAL, format, std::forward<Args>(args)...);
            std::exit(EXIT_FAILURE);
        }

        Logger& getLogger() { return logger_; }
        void setLogger(Logger& logger) { logger_ = std::move(logger); }

    private:
        template <typename... Args>
        void log(LogLevel level, const std::string& format, Args... args)
        {
            if (level < logger_.getLevel()) return;

            std::string message = formatMessage(format, std::forward<Args>(args)...);
            std::string timestamp = getCurrentTimestamp();
            std::string levelStr = getLevelString(level);

            std::string logMessage = logger_.getFormat();
            replaceAll(logMessage, "{timestamp}", timestamp);
            replaceAll(logMessage, "{name}", logger_.getName());
            replaceAll(logMessage, "{level}", levelStr);
            replaceAll(logMessage, "{message}", message);
            logMessage += "\n";

            outputToConsole(level, logMessage);
            outputToFile(logMessage);
        }

        void replaceAll(std::string& str, const std::string& from, const std::string& to)
        {
            size_t pos = 0;
            while ((pos = str.find(from, pos)) != std::string::npos)
            {
                str.replace(pos, from.length(), to);
                pos += to.length();
            }
        }

        std::string getCurrentTimestamp()
        {
            std::time_t now = std::time(nullptr);
            std::tm tm;
		#if defined(_MSC_VER) || defined(__MINGW32__)
	        localtime_s(&tm, &now);
	    #else
	        tm_ptr = localtime_r(&now, &tm);
	    #endif
            char buffer[80];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
            return buffer;
        }

        std::string getLevelString(LogLevel level)
        {
            static const std::map<LogLevel, std::string> levelStrings = {
                {LogLevel::DEBUG,    "DEBUG"},
                {LogLevel::INFO,     "INFO"},
                {LogLevel::WARNING,  "WARNING"},
                {LogLevel::ERROR,    "ERROR"},
                {LogLevel::CRITICAL, "CRITICAL"}
            };
            return levelStrings.at(level);
        }

        template <typename... Args>
        std::string formatMessage(const std::string& format, Args... args)
        {
            int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
            if (size_s <= 0) return "";
            auto size = static_cast<size_t>(size_s);
            std::unique_ptr<char[]> buf(new char[size]);
            std::snprintf(buf.get(), size, format.c_str(), args...);
            return std::string(buf.get(), buf.get() + size - 1);
        }

        void outputToConsole(LogLevel level, const std::string& message)
        {
            if (!logger_.isConsoleOutputEnabled()) return;

            if (logger_.isColorEnabled())
            {
                auto color = logger_.getLevelColors().at(level);
                std::cout << "\033[" << static_cast<int>(color) << "m"
                    << message << "\033[0m";
            }
            else
            {
                std::cout << message;
            }
            std::cout.flush();
        }

        void outputToFile(const std::string& message)
        {
            auto& file = logger_.getOutputFile();
            if (file.is_open())
            {
                file << message;
                file.flush();
            }
        }

        Logger& logger_;
    };

} // namespace cnt

#ifdef _CNT_LOGGING_SAVED_ERROR_DEFINE_
#define ERROR _CNT_LOGGING_SAVED_ERROR_DEFINE_
#undef _CNT_LOGGING_SAVED_ERROR_DEFINE_
#endif
