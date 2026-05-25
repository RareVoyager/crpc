#include <iostream>
#include <unistd.h>

#include <include/crpcapplication.h>

void ShowArgHelp()
{
}


namespace crpc
{
	CrpcConfig CrpcApplication::config_;
	void CrpcApplication::Init(int argc, char** argv)
	{
		// 读取config.conf
		if (argc < 2)
		{
			ShowArgHelp();
			exit(EXIT_FAILURE);
		}

		int c = 0;
		std::string config_file;
		// argv 里查找 -i 这个选项，并且 -i 后面必须带一个参数 参数保存到 optarg这个全局变量中
		while ((c = ::getopt(argc, argv, "i:")) != -1)
		{
			switch (c)
			{
			case 'i':
				config_file = optarg;
				break;
			case '?':
				std::cout << "invalid args!" << std::endl;
				ShowArgHelp();
				exit(EXIT_FAILURE);
			case ':':
				std::cout << "need <configfile>" << std::endl;
				ShowArgHelp();
				exit(EXIT_FAILURE);
			default:
				break;
			}

			// todo: 读取配置
			config_.LoadConfigFile(config_file.c_str());

			std::cout << "rpcserverip:" << config_.Load("rpcserverip") << std::endl;
			std::cout << "rpcserverport:" << config_.Load("rpcserverport") << std::endl;
			std::cout << "zookeeperip" << config_.Load("zookeeperip") << std::endl;
			std::cout << "zookeeperport:" << config_.Load("zookeeperport") << std::endl;
		}
	}

	CrpcApplication& CrpcApplication::GetInstance()
	{
		static CrpcApplication application;
		return application;
	}

	CrpcConfig& CrpcApplication::GetConfig()
	{
		return config_;
	}

}// namespace crpc
