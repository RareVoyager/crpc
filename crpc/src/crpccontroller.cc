#include <include/crpccontroller.h>

namespace crpc
{
	void CrpcController::Reset()
	{
		failed_ = false;
		errText_ = "";
	}

	bool CrpcController::Failed() const
	{
		return failed_;
	}

	std::string CrpcController::ErrorText() const
	{
		return errText_;
	}

	void CrpcController::SetFailed(const std::string& reason)
	{
		failed_ = true;
		errText_ = reason;
	}

	void CrpcController::StartCancel()
	{
	}

	bool CrpcController::IsCanceled() const
	{
		return false;
	}

	void CrpcController::NotifyOnCancel(google::protobuf::Closure* callback)
	{
	}


}// namespace crpc
