#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>


namespace crpc
{
	template<typename T>
	class LockQueue
	{
	public:
		void Push(const T& data)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			queue_.push(data);
			condition_.notify_one();
		}
		T Pop()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			while (queue_.empty())
			{
				condition_.wait(lock);
			}

			T data = queue_.front();
			queue_.pop();
			return data;
		}

	private:
		std::queue<T> queue_;
		std::mutex mutex_;
		std::condition_variable condition_;
	};
}// namespace crpc
