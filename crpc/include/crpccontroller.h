#pragma once

#include <google/protobuf/service.h>

namespace crpc
{
	// 保存调用过程中的状态信息
	class CrpcController : public google::protobuf::RpcController
	{
	public:
		void Reset();
		bool Failed() const;
		std::string ErrorText() const;
		void SetFailed(const std::string& reason);

		void StartCancel();
		bool IsCanceled() const;
		void NotifyOnCancel(google::protobuf::Closure* callback);

	private:
		bool failed_;
		std::string errText_;
	};

}// namespace crpc
