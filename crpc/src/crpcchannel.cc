#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

#include <include/crpcapplication.h>
#include <include/crpcchannel.h>
#include <include/rpcheader.pb.h>


namespace crpc
{

	void CrpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
								 google::protobuf::RpcController* controller,
								 const google::protobuf::Message* request,
								 google::protobuf::Message* response,
								 google::protobuf::Closure* done)
	{
		const google::protobuf::ServiceDescriptor* sd = method->service();
		std::string service_name = std::string(sd->name());
		std::string method_name = std::string(method->name());
		uint32_t args_size = 0;
		std::string args_str;
		if (request->SerializeToString(&args_str))
		{
			args_size = args_str.size();
		}
		else
		{
			std::cout << "serialize request error! " << __FILE__ << ":" << __LINE__ << std::endl;
			return;
		}


		crpc::CrpcHeader rpcheader;
		rpcheader.set_service_name(service_name);
		rpcheader.set_method_name(method_name);
		rpcheader.set_args_size(args_size);

		uint32_t header_size = 0;
		std::string rcp_header_str;
		if (rpcheader.SerializeToString(&rcp_header_str))
		{
			header_size = rcp_header_str.size();
		}
		else
		{
			std::cout << "serialize rpc header error! " << __FILE__ << ":" << __LINE__ << std::endl;
		}

		std::string send_str;
		send_str.insert(0, (std::string((char*)&header_size, 0)));
		send_str += rcp_header_str;
		send_str += args_str;

		// 自定义socket连接
		int clientfd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == clientfd)
		{
			std::cout << "create socket fd errno" << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string ip = crpc::CrpcApplication::GetConfig().Load("rpcserverip");
		uint16_t port = std::stoi(crpc::CrpcApplication::GetConfig().Load("rpcserverport").c_str());

		sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.s_addr = ::inet_addr(ip.c_str());

		if (-1 == connect(clientfd, (sockaddr*)&server, sizeof(server)))
		{
			std::cout << "connect errno" << errno << std::endl;
			::close(clientfd);
			exit(EXIT_FAILURE);
		}

		if (-1 == send(clientfd, send_str.c_str(), send_str.size(), 0))
		{
			std::cout << "send errno" << errno << std::endl;
		}

		// 接受rpc请求的响应值
		char buf[1024] = {0};
		int recv_size = 0;
		if (-1 == (recv_size = recv(clientfd, buf, 1024, 0)))
		{
			std::cout << "recv errno" << errno << std::endl;
			::close(clientfd);
			return;
		}

		// // bug:
		// std::string rpc_response_str(buf, 0, recv_size);
		// if (!response->ParseFromString(rpc_response_str))
		if (!response->ParseFromArray(buf, recv_size))
		{
			std::cout << "prase errno" << buf << std::endl;
			::close(clientfd);
			return;
		}
		::close(clientfd);
	}


}// namespace crpc
