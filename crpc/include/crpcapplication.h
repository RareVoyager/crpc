#pragma once


// crpc框架基础类 初始化操作
class CrpcApplication
{
public:
	static void Init(int argc, char** argv);
    // 单例模式
	static CrpcApplication* GetCrpcApplication();

private:
	CrpcApplication();
	// 单例模式，将拷贝构造等删除
	CrpcApplication(const CrpcApplication&) = delete;
	CrpcApplication(const CrpcApplication&&) = delete;
	CrpcApplication operator=(const CrpcApplication&) = delete;
};