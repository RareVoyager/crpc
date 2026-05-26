#pragma once
#include <unordered_map>


#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/EventLoop.h>

namespace crpc
{
	// 框架提供的专门发布rpc服务的网络对象类
	class CrpcProvider
	{
	public:
		// 框架提供给为外部使用的，发布rpc方法的接口。所有rpc方法都集成自Service
		void NotifyService(google::protobuf::Service* service);

		// 启动rpc服务节点，提供rpc远程网络调用服务
		void Run();

	private:
		void OnConnection(const muduo::net::TcpConnectionPtr&);

		// 手动处理了粘包问题
		void OnMessage(const muduo::net::TcpConnectionPtr&,
					   muduo::net::Buffer*,
					   muduo::Timestamp);
		// Closure 回调操作
		void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);

	private:
		muduo::net::EventLoop loop_;

		/**
		 * 包括服务节点以及他的方法map
		 */
		struct ServiceInfo
		{
			google::protobuf::Service* service_;
			std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> methodMap_;
		};

		// key: 服务名 value 服务信息哈希表
		std::unordered_map<std::string, ServiceInfo>
				serviceInfoMap_;
	};

}// namespace crpc
