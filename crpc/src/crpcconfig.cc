#include <filesystem>
#include <fstream>
#include <include/crpcconfig.h>
#include <iostream>
// #if __cplusplus >= 201703L
// namespace fs = std::filesystem;
// #include <filesystem>
// #endif


namespace crpc
{
	// todo: 使用C++17 的filesystem 优化
	void CrpcConfig::LoadConfigFile(const char* config_file)
	{
		// #if __cplusplus >= 201703L
		// 		fs::path config_path(config_file);

		// 		if (!fs::exists(config_path))
		// 		{
		// 			std::cout << config_path << " is not exists" << std::endl;
		// 			std::exit(EXIT_FAILURE);
		// 		}

		// 		if (!fs::is_regular_file(config_path))
		// 		{
		// 			std::cout << config_path << " is not a regular file" << std::endl;
		// 			std::exit(EXIT_FAILURE);
		// 		}

		// 		std::ifstream ifs(config_path);
		// 		if (!ifs.is_open())
		// 		{
		// 			std::cout << "open " << config_path << " failed" << std::endl;
		// 			std::exit(EXIT_FAILURE);
		// 		}

		// 		std::string line;
		// 		while (std::getline(ifs, line))
		// 		{
		// 			trim(line);

		// 			if (line.empty() || line[0] == '#')
		// 			{
		// 				continue;
		// 			}

		// 			auto pos = line.find('=');
		// 			if (pos == std::string::npos)
		// 			{
		// 				continue;
		// 			}

		// 			std::string key = line.substr(0, pos);
		// 			std::string value = line.substr(pos + 1);

		// 			trim(key);
		// 			trim(value);

		// 			if (!key.empty())
		// 			{
		// 				configMap_[key] = value;
		// 			}
		// 		}
		// #else
		// #endif
		FILE* fp = fopen(config_file, "r");
		if (nullptr == fp)
		{
			std::cout << config_file << " is not exits" << std::endl;
			exit(EXIT_FAILURE);
		}

		// todo: std::getline() fstream 优化
		// 去除注释 去除空格 =
		while (!feof(fp))
		{
			char buf[512] = {0};
			fgets(buf, 512, fp);
			std::string read_buf(buf);
			trim(read_buf);

			// 是注释
			if (read_buf.empty() || read_buf[0] == '#')
			{
				continue;
			}

			int idx = read_buf.find('=');
			if (idx == -1)
			{
				continue;
			}
			std::string key = read_buf.substr(0, idx);
			trim(key);
			int endidx = read_buf.find('\n', idx);
			std::string value = read_buf.substr(idx + 1, endidx - idx - 1);
			configMap_[key] = value;
		}
	}
	// todo: 使用C++17 的optional 优化

	std::string CrpcConfig::Load(const std::string& key)
	{
		if (configMap_.find(key) == configMap_.end())
		{
			return "";
		}
		return configMap_.find(key)->second;
	}

	void CrpcConfig::trim(std::string& str)
	{

		int idx = str.find_first_not_of(' ');
		if (idx != std::string::npos)
		{
			str = str.substr(idx, str.size() - idx);
		}
		idx = str.find_last_not_of(' ');
		if (idx != std::string::npos)
		{
			str = str.substr(0, idx + 1);
		}
	}


}// namespace crpc
