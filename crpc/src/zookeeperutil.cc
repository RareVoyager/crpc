#include <iostream>
#include <memory>

#include "include/crpcapplication.h"
#include "include/zookeeperutil.h"

namespace crpc
{
	namespace
	{
		struct CreateContext
		{
			zhandle_t* handle;
			std::string path;
			std::string data;
			int state;
		};

		struct GetContext
		{
			sem_t sem;
			int rc;
			std::string data;
		};

		void create_completion(int rc, const char* name, const void* data)
		{
			std::unique_ptr<CreateContext> ctx(static_cast<CreateContext*>(const_cast<void*>(data)));
			if (rc == ZOK)
			{
				std::cout << "znode create success... path:" << (name != nullptr ? name : ctx->path.c_str()) << std::endl;
			}
			else if (rc == ZNODEEXISTS)
			{
				std::cout << "znode already exists... path:" << ctx->path << std::endl;
			}
			else
			{
				std::cout << "flag:" << rc << std::endl;
				std::cout << "znode create error... path:" << ctx->path << std::endl;
			}
		}

		void exists_completion(int rc, const struct Stat*, const void* data)
		{
			std::unique_ptr<CreateContext> ctx(static_cast<CreateContext*>(const_cast<void*>(data)));
			if (rc == ZOK)
			{
				return;
			}

			if (rc != ZNONODE)
			{
				std::cout << "flag:" << rc << std::endl;
				std::cout << "znode exists check error... path:" << ctx->path << std::endl;
				return;
			}

			CreateContext* raw_ctx = ctx.release();
			int flag = zoo_acreate(raw_ctx->handle,
								   raw_ctx->path.c_str(),
								   raw_ctx->data.data(),
								   static_cast<int>(raw_ctx->data.size()),
								   &ZOO_OPEN_ACL_UNSAFE,
								   raw_ctx->state,
								   create_completion,
								   raw_ctx);
			if (flag != ZOK)
			{
				std::unique_ptr<CreateContext> failed_ctx(raw_ctx);
				std::cout << "flag:" << flag << std::endl;
				std::cout << "znode async create submit error... path:" << failed_ctx->path << std::endl;
			}
		}

		void get_completion(int rc, const char* value, int value_len, const struct Stat*, const void* data)
		{
			GetContext* ctx = static_cast<GetContext*>(const_cast<void*>(data));
			ctx->rc = rc;
			if (rc == ZOK && value != nullptr && value_len > 0)
			{
				ctx->data.assign(value, value_len);
			}
			sem_post(&ctx->sem);
		}
	}// namespace

	void global_watcher(zhandle_t* zh, int type,
						int state, const char* path, void* watcherCtx)
	{
		// 回调的消息类型是和会话相关的消息类型
		if (type == ZOO_SESSION_EVENT)
		{
			if (state == ZOO_CONNECTED_STATE)
			{
				// zkclient和zkserver连接成功
				sem_t* sem = (sem_t*)zoo_get_context(zh);
				if (sem != nullptr)
				{
					sem_post(sem);
				}
			}
		}
	}
	ZkClient::ZkClient() : _zhandle(nullptr)
	{
	}

	ZkClient::~ZkClient()
	{
		if (_zhandle != nullptr)
		{
			zookeeper_close(_zhandle);
		}
	}

	void ZkClient::start()
	{
		std::string ip = CrpcApplication::GetConfig().Load("zookeeperip");
		std::string port = CrpcApplication::GetConfig().Load("zookeeperport");
		std::string connstr = ip + ":" + port;

		/**
         * 这里用的是zookeeper_mt(多线程)
         * 1.调用api线程
         * 2.网络I/O线程(poll)
         * 3.watcher回调线程
         */
		_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);

		// 这里是可能的原因有内存不够用。init 是一个异步方法，并不是立即放回结果
		if (nullptr == _zhandle)
		{
			std::cout << "zookeeper_init error!" << std::endl;
			exit(EXIT_FAILURE);
		}

		sem_t sem;
		sem_init(&sem, 0, 0);
		zoo_set_context(_zhandle, &sem);

		sem_wait(&sem);
		zoo_set_context(_zhandle, nullptr);
		sem_destroy(&sem);
		std::cout << "zookeeper_init success!" << std::endl;
	}


	void ZkClient::create(const char* path, const char* data, int datalen, int state)
	{
		auto* ctx = new CreateContext;
		ctx->handle = _zhandle;
		ctx->path = path;
		ctx->state = state;
		if (data != nullptr && datalen > 0)
		{
			ctx->data.assign(data, datalen);
		}

		int flag = zoo_aexists(_zhandle, ctx->path.c_str(), 0, exists_completion, ctx);
		if (flag != ZOK)
		{
			std::unique_ptr<CreateContext> failed_ctx(ctx);
			std::cout << "flag:" << flag << std::endl;
			std::cout << "znode async exists submit error... path:" << failed_ctx->path << std::endl;
		}
	}

	std::string ZkClient::GetData(const char* path)
	{
		GetContext ctx;
		ctx.rc = ZOK;
		sem_init(&ctx.sem, 0, 0);

		int flag = zoo_aget(_zhandle, path, 0, get_completion, &ctx);
		if (flag != ZOK)
		{
			sem_destroy(&ctx.sem);
			std::cout << "get znode error... path:" << path << std::endl;
			return "";
		}

		sem_wait(&ctx.sem);
		sem_destroy(&ctx.sem);

		if (ctx.rc != ZOK)
		{
			std::cout << "get znode error... path:" << path << std::endl;
			return "";
		}

		return ctx.data;
	}

}// namespace crpc
