#include <iostream>


#include <include/crpcapplication.h>
#include <include/crpcprovider.h>
#include <include/rpcheader.pb.h>
#include <muduo/net/TcpServer.h>
namespace crpc
{
	void CrpcProvider::NotifyService(google::protobuf::Service* service)
	{
		ServiceInfo service_info;

		// google::protobuf::ServiceDescriptor 类型
		const auto service_descriptor = service->GetDescriptor();

		// 服务的名字
		std::string service_name = std::string(service_descriptor->name());
		// 服务的方法数量
		int methodCnt = service_descriptor->method_count();

		for (int i = 0; i < methodCnt; i++)
		{
			const google::protobuf::MethodDescriptor* method = service_descriptor->method(i);
			std::string methodName = std::string(method->name());
			service_info.methodMap_.insert({methodName, method});
		}
		service_info.service_ = service;
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


	/**
	 *  在框架内部，RpcProvider和RpcConsumer协商好之间通信用的protobuf数据类型
		service_name method_name args
		定义proto的message类型，进行数据头的序列化和反序列化service_name method_name args_size
		16UserServiceLoginzhang san123456
		header_size(4个字节) + header_str + args_str*
		已建立连接用户的读写事件回调如果远程有一个rpc服务的调用请求，那么onMessage方法就会响应
	 */
	void CrpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
								 muduo::net::Buffer* buffer,
								 muduo::Timestamp reciveTime)
	{
		// 获取传入的所有信息
		std::string recv_buf = buffer->retrieveAllAsString();

		// 存储
		uint32_t header_size = 0;
		// 将原始二进制数据拷贝到header_size
		recv_buf.copy((char*)&header_size, 4, 0);

		// 解析method_name service_name
		std::string header_str = recv_buf.substr(4, header_size);
		crpc::CrpcHeader crpcHeader;
		std::string service_name;
		std::string method_name;
		uint32_t args_size = 0;

		if (crpcHeader.ParseFromString(header_str))
		{
			service_name = crpcHeader.service_name();
			method_name = crpcHeader.method_name();
			args_size = crpcHeader.args_size();
		}
		else
		{
			std::cout << " header_str " << header_str << "prase error !" << std::endl;
			return;
		}

		// 获取参数列表
		std::string args_str = recv_buf.substr(4 + header_size, args_size);

		// 获取service 对象和method对象
		// 查找map中有没有这样的一个service_name 的服务
		auto it = serviceInfoMap_.find(service_name);
		if (it == serviceInfoMap_.end())
		{
			std::cout << service_name << "not find !" << std::endl;
			return;
		}
		// 查找这个服务下的节点是否有这样的方法
		auto mit = it->second.methodMap_.find(method_name);
		if (mit == it->second.methodMap_.end())
		{
			std::cout << service_name << ":" << service_name << "not find !" << std::endl;
			return;
		}

		// 获取service 和method对象
		google::protobuf::Service* service = it->second.service_;
		const google::protobuf::MethodDescriptor* method = mit->second;

		// 生成请求参数和返回参数
		google::protobuf::Message* request = service->GetRequestPrototype(method).New();
		if (!request->ParseFromString(args_str))
		{
			std::cout << "response parse errno!" << std::endl;
		}

		google::protobuf::Message* response = service->GetResponsePrototype(method).New();

		google::protobuf::Closure* done = google::protobuf::NewCallback<CrpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>(this, &CrpcProvider::SendRpcResponse, conn, response);

		service->CallMethod(method, nullptr, request, response, done);
	}


	// done 执行的run
	void CrpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
	{
		std::string response_str;
		if (response->SerializeToString(&response_str))
		{
			conn->send(response_str);
		}
		else
		{
			std::cout << "errno!" << std::endl;
		}
		conn->shutdown();
	}
}// namespace crpc
