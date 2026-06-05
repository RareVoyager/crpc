#pragma once

#include <semaphore.h>
#include <string>


#include <zookeeper/zookeeper.h>

namespace crpc
{
	class ZkClient
	{
	public:
		ZkClient();
		~ZkClient();

		/**
         * @brief: zkclient 启动客户端
         */
		void start();
		/**
         * @brief: 根据path创建znode节点
         * @param path: 节点路径
         * @param data: 节点数据
         * @param datalen: 数据长度
         * @param state 临时节点还是永久节点
         */
		void create(const char* path, const char* data, int datalen, int state = 0);

        /**
         * @brief: 根据路径获取数据
         */
		std::string GetData(const char* path);

	private:
		// zk客户端句柄
		zhandle_t* _zhandle;
	};
}// namespace crpc
