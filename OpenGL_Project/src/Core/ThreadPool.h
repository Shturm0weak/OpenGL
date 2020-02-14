#pragma once
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>
using Task = std::function<void()>;
class ThreadPool {
private:
	static ThreadPool* thread_pool;
	std::vector<std::thread> m_Threads;
	std::mutex m_mutex;
	std::condition_variable m_condition_var;
	std::queue <Task> m_Tasks;
	int num_Threads = 0;
	void Infinite_loop_function();
	bool m_stopping = false;
	static bool initialized;
public:

	ThreadPool(int n);
	~ThreadPool();
	void shutdown()noexcept;
	static ThreadPool* Instance() { return thread_pool; }
	void enqueue(Task task);
	static void Init();
};
