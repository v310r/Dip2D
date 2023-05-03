#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <functional>
#include <future>
#include <deque>


template<typename T>
class ThreadsafeQueue
{
public:
	struct Node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<Node> next;
	};

	ThreadsafeQueue() :
		head(new Node), tail(head.get())
	{}
	ThreadsafeQueue(const ThreadsafeQueue& other) = delete;
	ThreadsafeQueue& operator=(const ThreadsafeQueue& other) = delete;

	void push(T new_value)
	{
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
		std::unique_ptr<Node> p(new Node);
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);
			tail->data = new_data;
			Node* const new_tail = p.get();
			tail->next = std::move(p);
			tail = new_tail;
		}

		data_cond.notify_one();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_ptr<Node> old_head = wait_pop_head();
		return old_head->data;
	}

	void wait_and_pop(T& value)
	{
		std::unique_ptr<Node> old_head = wait_pop_head(value);
	}

	std::shared_ptr<T> try_pop()
	{
		std::unique_ptr<Node> old_head = try_pop_head();
		return old_head ? old_head->data : std::shared_ptr<T>();
	}

	bool try_pop(T& value)
	{
		std::unique_ptr<Node> old_head = try_pop_head(value);
		return old_head.get();
	}

	std::unique_lock<std::mutex> wait_for_data()
	{
		std::unique_lock<std::mutex> head_lock(head_mutex);
		data_cond.wait(head_lock, [&] {return head != get_tail(); });
		return std::move(head_lock);
	}


	bool empty()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		return (head == get_tail());
	}

private:
	Node* get_tail()
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		return tail;
	}

	std::unique_ptr<Node> pop_head()
	{
		std::unique_ptr<Node> old_head = std::move(head);
		head = std::move(old_head->next);
		return old_head;
	}

	std::unique_ptr<Node> wait_pop_head()
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		return pop_head();
	}

	std::unique_ptr<Node> wait_pop_head(T& value)
	{
		std::unique_lock<std::mutex> head_lock(wait_for_data());
		value = std::move(*head->data);
		return pop_head();
	}

	std::unique_ptr<Node> try_pop_head()
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<Node>();
		}
		return pop_head();
	}

	std::unique_ptr<Node> try_pop_head(T& value)
	{
		std::lock_guard<std::mutex> head_lock(head_mutex);
		if (head.get() == get_tail())
		{
			return std::unique_ptr<Node>();
		}
		value = std::move(*head->data);
		return pop_head();
	}

	std::mutex head_mutex;
	std::mutex tail_mutex;
	std::condition_variable data_cond;

	std::unique_ptr<Node> head;
	Node* tail;
};

class join_threads
{
public:
	explicit join_threads(std::vector<std::thread>& threads_) :
		threads(threads_)
	{

	}

	~join_threads()
	{
		for (std::thread& t : threads)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
	}

private:
	std::vector<std::thread>& threads;
};

class FunctionWrapper
{
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	std::unique_ptr<impl_base> impl;

	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
	};
public:
	template<typename F>
	FunctionWrapper(F&& f) :
		impl(new impl_type<F>(std::move(f)))
	{}

	void operator()() { impl->call(); }

	FunctionWrapper() = default;

	FunctionWrapper(FunctionWrapper&& other) :
		impl(std::move(other.impl))
	{}

	FunctionWrapper& operator=(FunctionWrapper&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}

	FunctionWrapper(const FunctionWrapper&) = delete;
	FunctionWrapper(FunctionWrapper&) = delete;
	FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};

class ThreadPool
{
public:
	ThreadPool() :
		done(false), joiner(threads)
	{
		unsigned const thread_count = std::thread::hardware_concurrency();
		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
				threads.push_back(
					std::thread(&ThreadPool::worker_thread, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~ThreadPool()
	{
		done = true;
	}

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type>
		submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type result_type;

		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push_back(std::move(task));
		return res;
	}

private:
	void worker_thread()
	{
		while (!done)
		{
			FunctionWrapper task;
			if (work_queue.try_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

	std::atomic_bool done;
	ThreadsafeQueue<FunctionWrapper> work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;
};