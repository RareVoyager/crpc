
#include <include/crpcapplication.h>
#include <include/crpcprovider.h>
#include <muduo/net/TcpServer.h>

namespace crpc
{
	void CrpcProvider::NotifyService(google::protobuf::Service* service)
	{
		ServiceInfo service_info;
		// google::protobuf::ServiceDescriptor 类型
		const auto service_descriptor = service->GetDescriptor();

		// 服务的名字
		std::string service_name = service_descriptor->name();
		// 服务的方法数量
		int methodCnt = service_descriptor->method_count();

		for (int i = 0; i < methodCnt; i++)
		{
			const google::protobuf::MethodDescriptor* method = service_descriptor->method(i);
			std::string methodName = method->name();
			service_info.methodMap_.insert({methodName, method});
		}

		serviceInfoMap_.insert({service_name, service_info});
	}

	void CrpcProvider::Run()
	{
		std::string ip = CrpcApplication::GetConfig().Load("rpcserverip");
		uint16_t port = stoi(CrpcApplication::GetConfig().Load("rpcserverport"));
		muduo::net::InetAddress address(ip, port, false);
		muduo::net::TcpServer server(&loop_, address, "rpcProvider");

		server.setThreadNum(3);

		server.setConnectionCallback([this](const muduo::net::TcpConnectionPtr& conn) {
			OnConnection(conn);
		});

		server.setMessageCallback([this](const muduo::net::TcpConnectionPtr& conn,
										 muduo::net::Buffer* buffer,
										 muduo::Timestamp reciveTime) {
			OnMessage(conn, buffer, reciveTime);
		});
		server.start();
		loop_.loop();
	}

	void CrpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn)
	{
	}


	void CrpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
								 muduo::net::Buffer* buffer,
								 muduo::Timestamp reciveTime)
	{
	}
}// namespace crpc
