#pragma once

#include <string>

#include "include/lockqueue.hpp"

namespace crpc
{
	enum LoggerLevel
	{
		INFO,
		ERROR,
	};
	class Logger
	{
	public:
		static Logger& GetInstance();
		void setLevel(LoggerLevel level);
		void Log(std::string str);

	private:
		// 日志等级
		int level_;
		LockQueue<std::string> lockQueue_;

		// 单例模式
		Logger();
		Logger(const Logger&) = delete;
		Logger(const Logger&&) = delete;
		Logger operator=(const Logger&) = delete;
	};
}// namespace crpc


#define LOG_INFO(logmsgformat, ...)                                    \
	do                                                                 \
	{                                                                  \
		crpc::Logger& logger = crpc::Logger::GetInstance();            \
		logger.setLevel(crpc::INFO);                                   \
		char c[1024] = {0};                                            \
		snprintf(c, 1024, "[INFO] " logmsgformat "\n", ##__VA_ARGS__); \
		logger.Log(c);                                                 \
	} while (0)

#define LOG_ERROR(logmsgformat, ...)                                    \
	do                                                                  \
	{                                                                   \
		crpc::Logger& logger = crpc::Logger::GetInstance();             \
		logger.setLevel(crpc::ERROR);                                   \
		char c[1024] = {0};                                             \
		snprintf(c, 1024, "[ERROR] " logmsgformat "\n", ##__VA_ARGS__); \
		logger.Log(c);                                                  \
	} while (0)