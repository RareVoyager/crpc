#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>

int main()
{
	muduo::net::EventLoop loop;

	muduo::net::InetAddress peerAddr(8000);
	muduo::net::TcpServer server(&loop, peerAddr, "myserver");
	server.setThreadNum(1);
	server.start();
	loop.loop();

	return 0;
}