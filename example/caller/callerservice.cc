#include <iostream>

#include <include/crpcapplication.h>
#include <include/crpcchannel.h>
#include <include/crpccontroller.h>

#include "user.pb.h"

int main(int argc, char** argv)
{

	crpc::CrpcApplication::Init(argc, argv);

	fixbug::UserServiceRpc_Stub stub(new crpc::CrpcChannel());

	fixbug::LoginRequest request;
	request.set_name("zhang san");
	request.set_pwd("123");

	fixbug::LoginResponse response;

	crpc::CrpcController controller;
	stub.Login(&controller, &request, &response, nullptr);

	if (controller.Failed())
	{
		std::cout << controller.ErrorText() << std::endl;
	}
	else
	{
		if (0 == response.result().errcode())
		{
			std::cout << "login success: " << response.success() << std::endl;
		}
		else
		{
			std::cout << "login errno: " << response.result().errmsg() << std::endl;
		}
	}


	return 0;
}