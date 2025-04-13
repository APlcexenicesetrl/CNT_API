/**
 * @file cnt/logging.h
 * Copyright 2025, aplcexenicesetrl project
 * This project and document files are maintained by CNT Development Team (under the APlcexenicesetrl studio),
 * and according to the project license (MIT license) agreement,
 * the project and documents can be used, modified, merged, published, branched, etc.
 * provided that the project is developed and open-source maintained by CNT Development Team.
 * At the same time,
 * project and documents can be used for commercial purposes under the condition of informing the development source,
 * but it is not allowed to be closed source, but it can be partially source.
 *
 * The project and documents will be updated and maintained from time to time,
 * and any form of dispute event, CNT Development Team.
 * and APlcexicesetrl shall not be liable for any damages,
 * and any compensation shall not be borne by the APlcexenicesetrl studio.
 */
 /* Written by Anders Norlander <taim_way@aplcexenicesetrl.com> */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <map>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__unix__)
#include <unistd.h>
#endif

namespace cnt
{
    // 显式定义枚举类
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
            levelColors_()
        {
            levelColors_[LogLevel::DEBUG] = LogColor::CYAN;
            levelColors_[LogLevel::INFO] = LogColor::GREEN;
            levelColors_[LogLevel::WARNING] = LogColor::YELLOW;
            levelColors_[LogLevel::ERROR] = LogColor::RED;
            levelColors_[LogLevel::CRITICAL] = LogColor::MAGENTA;

#if defined(_WIN32)
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut != INVALID_HANDLE_VALUE) {
                DWORD dwMode = 0;
                GetConsoleMode(hOut, &dwMode);
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
#endif
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        Logger(Logger&& other) noexcept
            : name_(std::move(other.name_)),
            level_(other.level_),
            file_(std::move(other.file_)),
            format_(std::move(other.format_)),
            useColor_(other.useColor_),
            levelColors_(std::move(other.levelColors_))
        {
            other.file_.close();
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
                levelColors_ = std::move(other.levelColors_);
                other.file_.close();
            }
            return *this;
        }

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
            if (file_.is_open())
                file_.close();

            file_.open(filename, std::ios::app);
            if (!file_.is_open())
                std::cerr << "Failed to open log file: " << filename << std::endl;

            return *this;
        }

        const std::string& getName() const { return name_; }
        LogLevel getLevel() const { return level_; }
        const std::string& getFormat() const { return format_; }
        bool isColorEnabled() const { return useColor_; }
        const std::map<LogLevel, LogColor>& getLevelColors() const { return levelColors_; }
        std::ofstream& getOutputFile() { return file_; }

    private:
        std::string name_;
        LogLevel level_;
        std::ofstream file_;
        std::string format_;
        bool useColor_;
        std::map<LogLevel, LogColor> levelColors_;
    };

    class Logging
    {
    public:
        explicit Logging(Logger* logger) : logger_(*logger) {}
        ~Logging() { logger_.getOutputFile().close(); }

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
        }

        Logger& getLogger() { return logger_; }
        void setLogger(Logger& logger) { logger_ = logger; }

    private:
        template <typename... Args>
        void log(LogLevel level, const std::string& format, Args... args)
        {
            if (level < logger_.getLevel())
                return;

            std::string message = formatMessage(format, std::forward<Args>(args)...);
            std::string timestamp = getCurrentTimestamp();
            std::string levelStr = getLevelString(level);

            std::string logMessage = logger_.getFormat();
            replaceAll(logMessage, "{timestamp}", timestamp);
            replaceAll(logMessage, "{name}", logger_.getName());
            replaceAll(logMessage, "{level}", levelStr);
            replaceAll(logMessage, "{message}", message);
            logMessage += "\n";

            if (logger_.isColorEnabled())
            {
                const auto& levelColors = logger_.getLevelColors();
                auto it = levelColors.find(level);
                if (it != levelColors.end())
                {
                    std::cout << "\033[" << static_cast<int>(it->second) << "m"
                        << logMessage << "\033[0m";
                }
                else
                {
                    std::cout << logMessage;
                }
            }
            else
            {
                std::cout << logMessage;
            }

            auto& file = logger_.getOutputFile();
            if (file.is_open())
            {
                file << logMessage;
                file.flush();
            }
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
            std::tm tm{};
#if defined(_WIN32)
            localtime_s(&tm, &now);
#else
            localtime_r(&now, &tm);
#endif
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }

        std::string getLevelString(LogLevel level)
        {
            switch (level)
            {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::WARNING:
                return "WARNING";
            case LogLevel::ERROR:
                return "ERROR";
            case LogLevel::CRITICAL:
                return "CRITICAL";
            default:
                return "UNKNOWN";
            }
        }

        template <typename... Args>
        std::string formatMessage(const std::string& format, Args... args)
        {
            size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
            std::vector<char> buf(size);
            snprintf(buf.data(), size, format.c_str(), args...);
            return std::string(buf.data(), buf.data() + size - 1);
        }

    private:
        Logger& logger_;
    };

} // namespace cnt