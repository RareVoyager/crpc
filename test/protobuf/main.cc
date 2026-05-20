#include "login.pb.h"
#include <iostream>

void print(const std::string& msg)
{
	std::cout << msg << std::endl;
}

void test1()
{
	std::cout << "==============test1==============" << std::endl;
	login::LoginRequest req;

	req.set_name("chen");
	req.set_pwd("260611");

	std::string out;
	if (req.SerializeToString(&out))
	{
		print(out);
	}

	login::LoginRequest preq;

	if (preq.ParseFromString(out))
	{
		print(preq.name());
		print(preq.pwd());
	}
}

void test2()
{
	std::cout << "==============test2==============" << std::endl;

	// 序列化
	login::LoginResponse response;
	response.set_success(true);
	login::ResultCode* code = response.mutable_code();
	code->set_errcode(1);
	code->set_msg("errno");

	std::string out;
	if (response.SerializeToString(&out))
	{
		print(out);
	}

	login::LoginResponse res;
	if (res.ParseFromString(out))
	{
		std::cout << std::boolalpha << res.success() << std::endl;
		std::cout << res.mutable_code()->errcode() << std::endl;
		std::cout << res.mutable_code()->msg() << std::endl;
	}
}

void test3()
{
	std::cout << "==============test3==============" << std::endl;

	login::GetFriendListResponse friendList;
	friendList.mutable_result()->set_errcode(200);
	friendList.mutable_result()->set_msg("成功");
	login::User* user = friendList.add_user_list();
	user->set_age(18);
	user->set_name("wangjiarong");
	user->set_sex(login::User::Sex::User_Sex_Woman);

	login::User* user1 = friendList.add_user_list();
	user1->set_age(19);
	user1->set_name("chenanqi");
	user1->set_sex(login::User::Sex::User_Sex_Man);

	std::cout << "friendLits 数量" << friendList.user_list_size() << std::endl;

	std::cout << "============序列化==============" << std::endl;

	std::string out;
	if (friendList.SerializeToString(&out))
	{
		std::cout << friendList.DebugString() << std::endl;
	}

	std::cout << "============反序列化==============" << std::endl;

	login::GetFriendListResponse friendList1;

	if (friendList1.ParseFromString(out))
	{
		std::cout << friendList1.result().errcode() << std::endl;
		std::cout << friendList1.result().msg() << std::endl;

		for (int i = 0; i < friendList1.user_list_size(); i++)
		{
			const auto& fr = friendList1.user_list(i);
			std::cout << fr.name() << std::endl;
			std::cout << fr.age() << std::endl;
			std::cout << fr.sex() << std::endl;
		}
	}
}

int main()
{
	test1();
	test2();
	test3();

	return 0;
}