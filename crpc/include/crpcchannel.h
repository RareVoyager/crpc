#pragma once

#include <google/protobuf/service.h>


namespace crpc
{
	// 重写了RpcChannel 的CallMethod 方法。
	class CrpcChannel : public google::protobuf::RpcChannel
	{
	public:
		// 主要写了数据如何组装
		// (数据发送格式 header_size + serviceName + methodName + args_size + argsStr)
		// 具体发送请求的定义在 rpcheader.proto 中
		// 后续解析时根据我们自定义的规则解析可以得到请求的服务名和方法名以及相关参数
		void CallMethod(const google::protobuf::MethodDescriptor* method,
						google::protobuf::RpcController* controller,
						const google::protobuf::Message* request,
						google::protobuf::Message* response,
						google::protobuf::Closure* done);

	private:
	};

}// namespace crpc
