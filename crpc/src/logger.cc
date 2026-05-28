#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <time.h>

#include "include/logger.h"


namespace crpc
{
	Logger& Logger::GetInstance()
	{
		static Logger logger;
		return logger;
	}

	void Logger::setLevel(LoggerLevel level)
	{
		level_ = level;
	}

	void Logger::Log(std::string str)
	{
		lockQueue_.Push(str);
	}

	// C语言API + thread 实现的异步写入文件日志
	// Logger::Logger()
	// {
	// 	std::thread writeLogtask([&]() {
	// 		for (;;)
	// 		{
	// 			// 获取当前时间
	// 			time_t now = time(nullptr);
	// 			tm* nowtm = localtime(&now);

	// 			char file_name[128] = {0};
	// 			sprintf(file_name, "%d-%d-%d-log.log", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);

	// 			FILE* fp = fopen(file_name, "a+");
	// 			if (nullptr == fp)
	// 			{
	// 				std::cout << "log file:" << file_name << "open error!" << std::endl;
	// 				exit(EXIT_FAILURE);
	// 			}

	// 			std::string msg = lockQueue_.Pop();
	// 			fputs(msg.c_str(), fp);
	// 			fclose(fp);
	// 		}
	// 	});

	// 	// 分离线程/ 守护线程
	// 	writeLogtask.detach();
	// }

	Logger::Logger()
	{
		std::thread writeLogTask([this]() {
			for (;;)
			{
				time_t now = time(nullptr);
				tm* nowtm = localtime(&now);

				std::ostringstream oss;
				oss << (nowtm->tm_year + 1900) << '-'
					<< std::setw(2) << std::setfill('0') << (nowtm->tm_mon + 1) << '-'
					<< std::setw(2) << std::setfill('0') << nowtm->tm_mday
					<< "-log.log";

				// 创建日志所在的文件夹
				std::filesystem::path log_dir = "logs";
				std::filesystem::create_directories(log_dir);
				// 日志文件所在的文件夹
				std::filesystem::path file_path = log_dir / oss.str();

				std::ofstream ofs(file_path, std::ios::app);
				if (!ofs.is_open())
				{
					std::cerr << "log file: " << file_path << " open error!" << std::endl;
					std::exit(EXIT_FAILURE);
				}
				std::string msg = lockQueue_.Pop();
				char time_buf[128] = {0};
				sprintf(time_buf, "[%d-%d-%d]:", nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec);
				msg.insert(0, time_buf);
				msg.append("\n");
				ofs << msg;
			}
		});

		writeLogTask.detach();
	}


}// namespace crpc
