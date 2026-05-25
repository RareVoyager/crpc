#pragma once

#include <string>
#include <unordered_map>

namespace crpc
{
	class CrpcConfig
	{
	public:
		// 加载解析配置文件
		void LoadConfigFile(const char* config_file);

		// 根据key 返回value
		std::string Load(const std::string& key);

	private:
		void trim(std::string& str);

		std::unordered_map<std::string, std::string> configMap_;
	};

}// namespace crpc
