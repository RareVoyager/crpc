#include "user.pb.h"

#include <include/crpcapplication.h>
#include <include/crpcchannel.h>
#include <include/crpcprovider.h>

class UserService : public fixbug::UserServiceRpc
{
public:
	bool Login(const std::string& name, const std::string& pwd)
	{
		std::cout << "waiting DB" << std::endl;
		std::cout << "name = " << name << " pwd =" << pwd << std::endl;
		return true;
	}

	// 执行业务逻辑
	void Login(::google::protobuf::RpcController* controller,
			   const ::fixbug::LoginRequest* request,
			   ::fixbug::LoginResponse* response,
			   ::google::protobuf::Closure* done)
	{
		std::string name = request->name();
		std::string pwd = request->pwd();

		bool result = Login(name, pwd);
		response->set_success(result);
		response->mutable_result()->set_errcode(0);
		response->mutable_result()->set_errmsg("");

		done->Run();
	}
};


int main(int argc, char** argv)
{
	crpc::CrpcApplication::Init(argc, argv);
	crpc::CrpcProvider provider;
	provider.NotifyService(new UserService());
	provider.Run();

	return 0;
}