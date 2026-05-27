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
		controller->Reset();
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
			char buf[512] = {0};
			sprintf(buf, "serialize request error! %s:%s", __FILE__, __LINE__);
			controller->SetFailed(buf);
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
			char buf[512] = {0};
			sprintf(buf, "serialize rpc header error! %s:%s", __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}

		std::string send_str;
		send_str.insert(0, (std::string((char*)&header_size, 4)));
		send_str += rcp_header_str;
		send_str += args_str;

		// 自定义socket连接
		int clientfd = socket(AF_INET, SOCK_STREAM, 0);
		if (-1 == clientfd)
		{
			char buf[512] = {0};
			sprintf(buf, "create socket fd errno:%u %s:%s", errno, __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}

		std::string ip = crpc::CrpcApplication::GetConfig().Load("rpcserverip");
		uint16_t port = std::stoi(crpc::CrpcApplication::GetConfig().Load("rpcserverport").c_str());

		sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.s_addr = ::inet_addr(ip.c_str());

		if (-1 == connect(clientfd, (sockaddr*)&server, sizeof(server)))
		{
			::close(clientfd);
			char buf[512] = {0};
			sprintf(buf, "connect errno:%u %s:%s", errno, __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}

		if (-1 == send(clientfd, send_str.c_str(), send_str.size(), 0))
		{
			char buf[512] = {0};
			sprintf(buf, "send errno:%u %s:%s", errno, __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}

		// 接受rpc请求的响应值
		char buf[1024] = {0};
		int recv_size = 0;
		if (-1 == (recv_size = recv(clientfd, buf, 1024, 0)))
		{
			::close(clientfd);
			char buf[512] = {0};
			sprintf(buf, "recv errno:%u %s:%s", errno, __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}

		// // bug:
		// std::string rpc_response_str(buf, 0, recv_size);
		// if (!response->ParseFromString(rpc_response_str))
		if (!response->ParseFromArray(buf, recv_size))
		{
			::close(clientfd);
			char buf[512] = {0};
			sprintf(buf, "prase errno:%u %s:%s", errno, __FILE__, __LINE__);
			controller->SetFailed(buf);
			return;
		}
		::close(clientfd);
	}


}// namespace crpc
